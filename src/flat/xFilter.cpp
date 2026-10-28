// These cubic interpolation curves pass through zero and approximately one.
float xFilterCurvedInterp1(float in_t)
{
    float in_t2 = in_t * in_t;
    return 0.41830066f * (in_t2 * in_t) + (1.8470588f * in_t - 1.2653595f * in_t2);
}

float xFilterCurvedInterp2(float in_t)
{
    float in_t2 = in_t * in_t;
    return 0.52380955f * (in_t2 * in_t) + (2.459524f * in_t - 1.9833333f * in_t2);
}

float xFilterCurvedInterp3(float in_t)
{
    float in_t2 = in_t * in_t;
    return 1.3727273f * (in_t2 * in_t) + (3.1495454f * in_t - 3.5222728f * in_t2);
}

float xFilterCurvedInterp4(float in_t)
{
    float in_t2 = in_t * in_t;
    return 1.1692308f * (in_t2 * in_t) + (0.38564104f * in_t - 0.5548718f * in_t2);
}
