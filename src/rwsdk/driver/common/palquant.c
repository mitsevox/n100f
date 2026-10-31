#include <rwcore.h>

// The palette quantizer: images add their colours to a tree that splits each colour channel
// into 32 levels, and the palette is then chosen by cutting the colour space into boxes of
// least variance, four channels at a time.

// The depth of the tree: five levels of 16 children, one bit of each channel per level.
#define PALQUANT_DEPTH 5

// The most palette entries the quantizer resolves.
#define PALQUANT_MAXCOLORS 256

// The scale from a colour channel to the unit range, and from a mean back to a channel.
#define PALQUANT_TOUNIT (1.0f / 255.0f)
#define PALQUANT_TOCHANNEL 255.9999f

typedef struct PalQuantLeaf PalQuantLeaf;
struct PalQuantLeaf
{
    F32 weight;
    F32 red;
    F32 green;
    F32 blue;
    F32 alpha;
    F32 var;
    U8 palIndex;
};

typedef struct PalQuantBranch PalQuantBranch;
struct PalQuantBranch
{
    PalQuantBranch* child[16];
};

typedef struct PalQuantBox PalQuantBox;
struct PalQuantBox
{
    RwRGBA lower;
    RwRGBA upper;
};

typedef struct PalQuant PalQuant;
struct PalQuant
{
    PalQuantBox* boxes;
    F32* variances;
    PalQuantBranch* root;
    RwFreeList* nodeFreeList;
};

// Spreads the five bits of a channel's top five bits over the five levels of the tree.
static U32 splice[32];

#define PALQUANT_INDEX(_col)                                                                       \
    ((splice[(_col)->red >> 3] << 3) | (splice[(_col)->green >> 3] << 2) |                        \
     (splice[(_col)->blue >> 3] << 1) | splice[(_col)->alpha >> 3])

static void InitLeaf(PalQuantLeaf* leaf)
{
    leaf->palIndex = 0xFF;
    leaf->weight = 0.0f;
    leaf->red = 0.0f;
    leaf->green = 0.0f;
    leaf->blue = 0.0f;
    leaf->alpha = 0.0f;
    leaf->var = 0.0f;
}

static void InitBranch(PalQuantBranch* branch)
{
    S32 i;

    for (i = 0; i < 16; i++)
    {
        branch->child[i] = NULL;
    }
}

static PalQuantBranch* CreateCube(RwFreeList* freeList)
{
    return (PalQuantBranch*)RWSRCGLOBAL(memoryAlloc)(freeList);
}

// Finds the leaf for a colour, creating the branches and the leaf on the way.
static PalQuantLeaf* AllocateToLeaf(PalQuant* pq, PalQuantBranch* root, U32 cellIndex, S32 depth)
{
    if (depth == 0)
    {
        return (PalQuantLeaf*)root;
    }

    if (!root->child[cellIndex & 15])
    {
        root->child[cellIndex & 15] = CreateCube(pq->nodeFreeList);

        if (depth == 1)
        {
            InitLeaf((PalQuantLeaf*)root->child[cellIndex & 15]);
        }
        else
        {
            InitBranch(root->child[cellIndex & 15]);
        }
    }

    return AllocateToLeaf(pq, root->child[cellIndex & 15], cellIndex >> 4, depth - 1);
}

// Adds an image's colours to the tree with the given weight.
void _rwPalQuantAddImage(PalQuant* pq, RwImage* image, F32 weight)
{
    S32 depth = image->depth;
    S32 stride = image->stride;
    U8* pixels = image->cpPixels;
    RwRGBA* palette = image->palette;
    S32 y = image->height;

    switch (depth)
    {
    case 4:
    case 8:
        while (y--)
        {
            U8* pixel = pixels;
            S32 x = image->width;

            while (x--)
            {
                RwRGBA* color = &palette[*pixel];
                PalQuantLeaf* leaf =
                    AllocateToLeaf(pq, pq->root, PALQUANT_INDEX(color), PALQUANT_DEPTH);
                F32 red;
                F32 green;
                F32 blue;
                F32 alpha;

                pixel++;

                red = PALQUANT_TOUNIT * color->red * weight;
                green = PALQUANT_TOUNIT * color->green * weight;
                blue = PALQUANT_TOUNIT * color->blue * weight;
                alpha = PALQUANT_TOUNIT * color->alpha * weight;
                leaf->weight += weight;
                leaf->red += red;
                leaf->green += green;
                leaf->blue += blue;
                leaf->alpha += alpha;
                leaf->var += weight * (red * red + green * green + blue * blue + alpha * alpha);
            }

            pixels += stride;
        }
        break;
    case 32:
        while (y--)
        {
            RwRGBA* color = (RwRGBA*)pixels;
            S32 x = image->width;

            while (x--)
            {
                PalQuantLeaf* leaf =
                    AllocateToLeaf(pq, pq->root, PALQUANT_INDEX(color), PALQUANT_DEPTH);
                F32 red;
                F32 green;
                F32 blue;
                F32 alpha;

                red = PALQUANT_TOUNIT * color->red * weight;
                green = PALQUANT_TOUNIT * color->green * weight;
                blue = PALQUANT_TOUNIT * color->blue * weight;
                alpha = PALQUANT_TOUNIT * color->alpha * weight;
                leaf->weight += weight;
                leaf->red += red;
                leaf->green += green;
                leaf->blue += blue;
                leaf->alpha += alpha;
                leaf->var += weight * (red * red + green * green + blue * blue + alpha * alpha);

                color++;
            }

            pixels += stride;
        }
        break;
    }
}

// Gives every leaf inside a box the box's palette index.
static void assignindex(PalQuantBranch* root, RwRGBA* origin, S32 depth, PalQuantBox* region,
                        S32 palIndex)
{
    if (root)
    {
        S32 width = 1 << depth;
        S32 dr = origin->red - region->upper.red;
        S32 dg = origin->green - region->upper.green;
        S32 db = origin->blue - region->upper.blue;
        S32 da = origin->alpha - region->upper.alpha;

        if (dr < 0 && dg < 0 && db < 0 && da < 0)
        {
            S32 dR = region->lower.red - origin->red;
            S32 dG = region->lower.green - origin->green;
            S32 dB = region->lower.blue - origin->blue;
            S32 dA = region->lower.alpha - origin->alpha;

            if (dR < width && dG < width && dB < width && dA < width)
            {
                if (dr <= -width && dg <= -width && db <= -width && da <= -width && dR <= 0 &&
                    dG <= 0 && dB <= 0 && dA <= 0 && depth == 0)
                {
                    ((PalQuantLeaf*)root)->palIndex = (U8)palIndex;
                }
                else
                {
                    S32 i;

                    for (i = 0; i < 16; i++)
                    {
                        RwRGBA corner;

                        corner.red = origin->red + (((i >> 3) & 1) << (depth - 1));
                        corner.green = origin->green + (((i >> 2) & 1) << (depth - 1));
                        corner.blue = origin->blue + (((i >> 1) & 1) << (depth - 1));
                        corner.alpha = origin->alpha + ((i & 1) << (depth - 1));
                        assignindex(root->child[i], &corner, depth - 1, region, palIndex);
                    }
                }
            }
        }
    }
}

// Sums the weights and colours of the leaves inside a box.
static void addvolume(PalQuantBranch* root, RwRGBA* origin, S32 depth, PalQuantBox* region,
                      PalQuantLeaf* volume)
{
    if (root)
    {
        S32 width = 1 << depth;
        S32 dr = origin->red - region->upper.red;
        S32 dg = origin->green - region->upper.green;
        S32 db = origin->blue - region->upper.blue;
        S32 da = origin->alpha - region->upper.alpha;

        if (dr < 0 && dg < 0 && db < 0 && da < 0)
        {
            S32 dR = region->lower.red - origin->red;
            S32 dG = region->lower.green - origin->green;
            S32 dB = region->lower.blue - origin->blue;
            S32 dA = region->lower.alpha - origin->alpha;

            if (dR < width && dG < width && dB < width && dA < width)
            {
                if (dr <= -width && dg <= -width && db <= -width && da <= -width && dR <= 0 &&
                    dG <= 0 && dB <= 0 && dA <= 0 && depth == 0)
                {
                    PalQuantLeaf* leaf = (PalQuantLeaf*)root;

                    volume->weight += leaf->weight;
                    volume->red += leaf->red;
                    volume->green += leaf->green;
                    volume->blue += leaf->blue;
                    volume->alpha += leaf->alpha;
                    volume->var += leaf->var;
                }
                else
                {
                    S32 i;

                    for (i = 0; i < 16; i++)
                    {
                        RwRGBA corner;

                        corner.red = origin->red + (((i >> 3) & 1) << (depth - 1));
                        corner.green = origin->green + (((i >> 2) & 1) << (depth - 1));
                        corner.blue = origin->blue + (((i >> 1) & 1) << (depth - 1));
                        corner.alpha = origin->alpha + ((i & 1) << (depth - 1));
                        addvolume(root->child[i], &corner, depth - 1, region, volume);
                    }
                }
            }
        }
    }
}

// Sums the leaves inside a box into volume.
static void nVol(PalQuantBranch* root, PalQuantBox* region, PalQuantLeaf* volume)
{
    RwRGBA origin;

    origin.red = 0;
    origin.green = 0;
    origin.blue = 0;
    origin.alpha = 0;
    InitLeaf(volume);
    addvolume(root, &origin, PALQUANT_DEPTH, region, volume);
}

#define PALQUANT_DOT(_v)                                                                           \
    ((_v)->red * (_v)->red + (_v)->green * (_v)->green + (_v)->blue * (_v)->blue +                 \
     (_v)->alpha * (_v)->alpha)

// Tries every cut of a box along one channel and returns the best split's score, with the cut in
// *cut (-1 if no cut leaves weight on both sides).
static F32 nMaximize(PalQuantBranch* root, PalQuantBox* set, S32 dir, S32* cut,
                     PalQuantLeaf* whole)
{
    F32 max = 0.0f;
    F32 last = 0.0f;
    PalQuantBox sub;
    PalQuantLeaf half;
    S32 i;

    *cut = -1;
    sub = *set;

    switch (dir)
    {
    case 1:
        for (i = set->lower.red; i < set->upper.red; i++)
        {
            F32 restWeight;
            F32 restRed;
            F32 restGreen;
            F32 restBlue;
            F32 restAlpha;

            sub.upper.red = i;
            nVol(root, &sub, &half);
            restWeight = whole->weight - half.weight;
            restRed = whole->red - half.red;
            restGreen = whole->green - half.green;
            restBlue = whole->blue - half.blue;
            restAlpha = whole->alpha - half.alpha;

            if (half.weight > 0.0f && restWeight > 0.0f)
            {
                F32 temp = PALQUANT_DOT(&half) / half.weight +
                           (restRed * restRed + restGreen * restGreen + restBlue * restBlue +
                            restAlpha * restAlpha) /
                               restWeight;

                if (temp > max)
                {
                    max = temp;
                    *cut = i;
                }
                else if (temp < last)
                {
                    break;
                }

                last = temp;
            }
        }
        break;
    case 2:
        for (i = set->lower.green; i < set->upper.green; i++)
        {
            F32 restWeight;
            F32 restRed;
            F32 restGreen;
            F32 restBlue;
            F32 restAlpha;

            sub.upper.green = i;
            nVol(root, &sub, &half);
            restWeight = whole->weight - half.weight;
            restRed = whole->red - half.red;
            restGreen = whole->green - half.green;
            restBlue = whole->blue - half.blue;
            restAlpha = whole->alpha - half.alpha;

            if (half.weight > 0.0f && restWeight > 0.0f)
            {
                F32 temp = PALQUANT_DOT(&half) / half.weight +
                           (restRed * restRed + restGreen * restGreen + restBlue * restBlue +
                            restAlpha * restAlpha) /
                               restWeight;

                if (temp > max)
                {
                    max = temp;
                    *cut = i;
                }
                else if (temp < last)
                {
                    break;
                }

                last = temp;
            }
        }
        break;
    case 3:
        for (i = set->lower.blue; i < set->upper.blue; i++)
        {
            F32 restWeight;
            F32 restRed;
            F32 restGreen;
            F32 restBlue;
            F32 restAlpha;

            sub.upper.blue = i;
            nVol(root, &sub, &half);
            restWeight = whole->weight - half.weight;
            restRed = whole->red - half.red;
            restGreen = whole->green - half.green;
            restBlue = whole->blue - half.blue;
            restAlpha = whole->alpha - half.alpha;

            if (half.weight > 0.0f && restWeight > 0.0f)
            {
                F32 temp = PALQUANT_DOT(&half) / half.weight +
                           (restRed * restRed + restGreen * restGreen + restBlue * restBlue +
                            restAlpha * restAlpha) /
                               restWeight;

                if (temp > max)
                {
                    max = temp;
                    *cut = i;
                }
                else if (temp < last)
                {
                    break;
                }

                last = temp;
            }
        }
        break;
    case 4:
        for (i = set->lower.alpha; i < set->upper.alpha; i++)
        {
            F32 restWeight;
            F32 restRed;
            F32 restGreen;
            F32 restBlue;
            F32 restAlpha;

            sub.upper.alpha = i;
            nVol(root, &sub, &half);
            restWeight = whole->weight - half.weight;
            restRed = whole->red - half.red;
            restGreen = whole->green - half.green;
            restBlue = whole->blue - half.blue;
            restAlpha = whole->alpha - half.alpha;

            if (half.weight > 0.0f && restWeight > 0.0f)
            {
                F32 temp = PALQUANT_DOT(&half) / half.weight +
                           (restRed * restRed + restGreen * restGreen + restBlue * restBlue +
                            restAlpha * restAlpha) /
                               restWeight;

                if (temp > max)
                {
                    max = temp;
                    *cut = i;
                }
                else if (temp < last)
                {
                    break;
                }

                last = temp;
            }
        }
        break;
    }

    return max;
}

// Splits set1 along the channel whose best cut scores highest, putting the upper part in set2.
static S32 nCut(PalQuantBranch* root, PalQuantBox* set1, PalQuantBox* set2)
{
    S32 cuta;
    S32 cutb;
    S32 cutg;
    S32 cutr;
    PalQuantLeaf whole;
    F32 maxr;
    F32 maxg;
    F32 maxb;
    F32 maxa;

    nVol(root, set1, &whole);
    maxr = nMaximize(root, set1, 1, &cutr, &whole);
    maxg = nMaximize(root, set1, 2, &cutg, &whole);
    maxb = nMaximize(root, set1, 3, &cutb, &whole);
    maxa = nMaximize(root, set1, 4, &cuta, &whole);

    if (maxr <= 0.0f && maxg <= 0.0f && maxb <= 0.0f && maxa <= 0.0f)
    {
        return FALSE;
    }

    *set2 = *set1;

    if (maxr >= maxg)
    {
        if (maxr >= maxb)
        {
            if (maxr >= maxa)
            {
                set2->lower.red = cutr;
                set1->upper.red = cutr;
            }
            else
            {
                set2->lower.alpha = cuta;
                set1->upper.alpha = cuta;
            }
        }
        else if (maxb >= maxa)
        {
            set2->lower.blue = cutb;
            set1->upper.blue = cutb;
        }
        else
        {
            set2->lower.alpha = cuta;
            set1->upper.alpha = cuta;
        }
    }
    else if (maxg >= maxb)
    {
        if (maxg >= maxa)
        {
            set2->lower.green = cutg;
            set1->upper.green = cutg;
        }
        else
        {
            set2->lower.alpha = cuta;
            set1->upper.alpha = cuta;
        }
    }
    else if (maxb >= maxa)
    {
        set2->lower.blue = cutb;
        set1->upper.blue = cutb;
    }
    else
    {
        set2->lower.alpha = cuta;
        set1->upper.alpha = cuta;
    }

    return TRUE;
}

static S32 CountLeafs(PalQuantBranch* root, S32 depth)
{
    S32 count = 0;

    if (root)
    {
        if (depth > 0)
        {
            S32 i;

            for (i = 0; i < 16; i++)
            {
                count += CountLeafs(root->child[i], depth - 1);
            }
        }
        else
        {
            count = 1;
        }
    }

    return count;
}

// Writes each leaf's mean colour to the palette, numbering the leaves in tree order.
static S32 ExtractNodes(PalQuantBranch* root, RwRGBA* palette, S32 palIndex, S32 depth)
{
    if (root)
    {
        if (depth > 0)
        {
            S32 i;

            for (i = 0; i < 16; i++)
            {
                palIndex = ExtractNodes(root->child[i], palette, palIndex, depth - 1);
            }
        }
        else
        {
            PalQuantLeaf* leaf = (PalQuantLeaf*)root;
            F32 scale = 0.0f;

            if (leaf->weight > 0.0f)
            {
                scale = PALQUANT_TOCHANNEL / leaf->weight;
            }

            palette[palIndex].red = (U8)(leaf->red * scale);
            palette[palIndex].green = (U8)(leaf->green * scale);
            palette[palIndex].blue = (U8)(leaf->blue * scale);
            palette[palIndex].alpha = (U8)(leaf->alpha * scale);
            leaf->palIndex = (U8)palIndex;
            palIndex++;
        }
    }

    return palIndex;
}

// Chooses up to maxcols palette entries for the colours added so far and returns how many it
// chose; every leaf then knows its entry.
S32 _rwPalQuantResolvePalette(RwRGBA* palette, S32 maxcols, PalQuant* pq)
{
    S32 numcols = CountLeafs(pq->root, PALQUANT_DEPTH);

    if (numcols <= maxcols)
    {
        S32 i;

        numcols = ExtractNodes(pq->root, palette, 0, PALQUANT_DEPTH);

        for (i = numcols; i < maxcols; i++)
        {
            palette[i].red = 0;
            palette[i].green = 0;
            palette[i].blue = 0;
            palette[i].alpha = 0;
        }
    }
    else
    {
        PalQuantLeaf volume;
        S32 i;

        pq->boxes[0].lower.red = 0;
        pq->boxes[0].lower.green = 0;
        pq->boxes[0].lower.blue = 0;
        pq->boxes[0].lower.alpha = 0;
        pq->boxes[0].upper.red = 32;
        pq->boxes[0].upper.green = 32;
        pq->boxes[0].upper.blue = 32;
        pq->boxes[0].upper.alpha = 32;
        nVol(pq->root, &pq->boxes[0], &volume);
        pq->variances[0] = volume.var - PALQUANT_DOT(&volume) / volume.weight;

        for (i = 1; i < maxcols; i++)
        {
            F32 maxvv = 0.0f;
            S32 next = -1;
            S32 k;

            for (k = 0; k < i; k++)
            {
                if (pq->variances[k] > maxvv)
                {
                    maxvv = pq->variances[k];
                    next = k;
                }
            }

            if (next == -1)
            {
                break;
            }

            if (nCut(pq->root, &pq->boxes[next], &pq->boxes[i]))
            {
                PalQuantLeaf lower;
                PalQuantLeaf upper;

                nVol(pq->root, &pq->boxes[next], &lower);
                pq->variances[next] = lower.var - PALQUANT_DOT(&lower) / lower.weight;
                nVol(pq->root, &pq->boxes[i], &upper);
                pq->variances[i] = upper.var - PALQUANT_DOT(&upper) / upper.weight;
            }
            else
            {
                pq->variances[next] = 0.0f;
                i--;
            }
        }

        numcols = i;

        for (i = 0; i < maxcols; i++)
        {
            if (i < numcols)
            {
                RwRGBA origin;
                PalQuantLeaf mean;
                F32 scale;

                origin.red = 0;
                origin.green = 0;
                origin.blue = 0;
                origin.alpha = 0;
                assignindex(pq->root, &origin, PALQUANT_DEPTH, &pq->boxes[i], i);
                nVol(pq->root, &pq->boxes[i], &mean);

                scale = 0.0f;

                if (mean.weight > 0.0f)
                {
                    scale = PALQUANT_TOCHANNEL / mean.weight;
                }

                palette[i].red = (U8)(mean.red * scale);
                palette[i].green = (U8)(mean.green * scale);
                palette[i].blue = (U8)(mean.blue * scale);
                palette[i].alpha = (U8)(mean.alpha * scale);
            }
            else
            {
                palette[i].red = 0;
                palette[i].green = 0;
                palette[i].blue = 0;
                palette[i].alpha = 0;
            }
        }
    }

    return numcols;
}

static U8 GetIndex(PalQuantBranch* root, U32 cellIndex, S32 depth)
{
    if (depth == 0)
    {
        return ((PalQuantLeaf*)root)->palIndex;
    }

    return GetIndex(root->child[cellIndex & 15], cellIndex >> 4, depth - 1);
}

// Writes an image's pixels as palette indices, two to a byte for a 4-bit packed destination.
void _rwPalQuantMatchImage(U8* dstPixels, S32 dstStride, S32 dstDepth, S32 dstPacked,
                           PalQuant* pq, RwImage* image)
{
    S32 stride = image->stride;
    U8* pixels = image->cpPixels;

    if (dstDepth == 4 && dstPacked)
    {
        switch (image->depth)
        {
        case 4:
        case 8:
        {
            RwRGBA* palette = image->palette;
            U32 y = image->height;

            while (y--)
            {
                S32 width = image->width;
                U8* src = pixels;
                U8* dst = dstPixels;
                S32 x;

                for (x = 0; x < width; x++)
                {
                    RwRGBA* color = &palette[*src++];
                    U8 index = GetIndex(pq->root, PALQUANT_INDEX(color), PALQUANT_DEPTH);

                    if (x & 1)
                    {
                        *dst &= 0x0F;
                        *dst |= (index << 4) & 0xF0;
                        dst++;
                    }
                    else
                    {
                        *dst &= 0xF0;
                        *dst |= index & 0x0F;
                    }
                }

                pixels += stride;
                dstPixels += dstStride;
            }
            break;
        }
        case 32:
        {
            U32 y = image->height;

            while (y--)
            {
                S32 width = image->width;
                RwRGBA* color = (RwRGBA*)pixels;
                U8* dst = dstPixels;
                S32 x;

                for (x = 0; x < width; x++)
                {
                    U8 index = GetIndex(pq->root, PALQUANT_INDEX(color), PALQUANT_DEPTH);

                    color++;

                    if (x & 1)
                    {
                        *dst &= 0x0F;
                        *dst |= (index << 4) & 0xF0;
                        dst++;
                    }
                    else
                    {
                        *dst &= 0xF0;
                        *dst |= index & 0x0F;
                    }
                }

                pixels += stride;
                dstPixels += dstStride;
            }
            break;
        }
        }
    }
    else
    {
        switch (image->depth)
        {
        case 4:
        case 8:
        {
            RwRGBA* palette = image->palette;
            U32 y = image->height;

            while (y--)
            {
                U32 x = image->width;
                U8* src = pixels;
                U8* dst = dstPixels;

                while (x--)
                {
                    RwRGBA* color = &palette[*src++];

                    *dst++ = GetIndex(pq->root, PALQUANT_INDEX(color), PALQUANT_DEPTH);
                }

                pixels += stride;
                dstPixels += dstStride;
            }
            break;
        }
        case 32:
        {
            U32 y = image->height;

            while (y--)
            {
                U32 x = image->width;
                RwRGBA* color = (RwRGBA*)pixels;
                U8* dst = dstPixels;

                while (x--)
                {
                    *dst++ = GetIndex(pq->root, PALQUANT_INDEX(color), PALQUANT_DEPTH);
                    color++;
                }

                pixels += stride;
                dstPixels += dstStride;
            }
            break;
        }
        }
    }
}

// Sets up the quantizer: the channel splice table, the box and variance arrays and an empty tree.
S32 _rwPalQuantInit(PalQuant* pq)
{
    S32 i;

    for (i = 0; i < 32; i++)
    {
        U32 value = 0;
        S32 bit;

        for (bit = 0; bit < PALQUANT_DEPTH; bit++)
        {
            value |= (i & (1 << bit)) ? (1 << ((PALQUANT_DEPTH - 1 - bit) * 4)) : 0;
        }

        splice[i] = value;
    }

    pq->boxes = (PalQuantBox*)RWSRCGLOBAL(memoryFuncs).rwcalloc(sizeof(PalQuantBox),
                                                                 PALQUANT_MAXCOLORS);
    pq->variances = (F32*)RWSRCGLOBAL(memoryFuncs).rwcalloc(sizeof(F32), PALQUANT_MAXCOLORS);
    pq->nodeFreeList = RwFreeListCreate(sizeof(PalQuantBranch), 64, 0);
    pq->root = CreateCube(pq->nodeFreeList);
    InitBranch(pq->root);

    return TRUE;
}

static void DeleteOctTree(PalQuant* pq, PalQuantBranch* root, S32 depth)
{
    if (root)
    {
        if (depth > 0)
        {
            S32 i;

            for (i = 0; i < 16; i++)
            {
                DeleteOctTree(pq, root->child[i], depth - 1);
            }
        }

        RWSRCGLOBAL(memoryFree)(pq->nodeFreeList, root);
    }
}

void _rwPalQuantTerm(PalQuant* pq)
{
    DeleteOctTree(pq, pq->root, PALQUANT_DEPTH);
    pq->root = NULL;
    RwFreeListDestroy(pq->nodeFreeList);
    RwFree(pq->variances);
    RwFree(pq->boxes);
}
