#include "zFeet.h"
#include "xMemMgr.h"

// Age the oldest edge until the strip has fewer than two edges left.
void zFeetUpdateSkidMark(_tagFeetSkidMark* m, F32 seconds)
{
    if (m->total_edges > 0)
    {
        m->remove_timer -= seconds;
        if (m->remove_timer <= 0.0f)
        {
            m->remove_timer = 1.0f;
            m->first_idx++;
            if (m->first_idx >= m->total_edges - 1)
            {
                m->first_idx = 0;
                m->total_edges = 0;
            }
        }
    }
}

// Allocate a sixteen-edge strip for each foot.
void zFeetInitSkidMark(_tagFeetSkidMark* m, S32 total_feet)
{
    m->total_feet = total_feet;
    for (S32 i = 0; i < m->total_feet; i++)
    {
        m->edge[i] = (_tagSkidEdge*)xMemAlloc(gActiveHeap, 16 * sizeof(_tagSkidEdge));
    }
    m->first_idx = 0;
    m->total_edges = 0;
}
