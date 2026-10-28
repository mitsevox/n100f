// These cubic interpolation curves pass through zero and approximately one.
float xFilterCurvedInterp1(float t)
{
    float t2 = t * t;
    float t3 = t2 * t;
    return 0.418300658f * t3 + (1.84705877f * t - 1.26535952f * t2);
}

float xFilterCurvedInterp2(float t)
{
    float t2 = t * t;
    float t3 = t2 * t;
    return 0.523809552f * t3 + (2.45952392f * t - 1.98333335f * t2);
}

float xFilterCurvedInterp3(float t)
{
    float t2 = t * t;
    float t3 = t2 * t;
    return 1.37272727f * t3 + (3.14954543f * t - 3.52227283f * t2);
}

float xFilterCurvedInterp4(float t)
{
    float t2 = t * t;
    float t3 = t2 * t;
    return 1.16923082f * t3 + (0.385641038f * t - 0.554871798f * t2);
}
