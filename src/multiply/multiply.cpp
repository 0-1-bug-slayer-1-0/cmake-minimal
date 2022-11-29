#include <multiply/multiply.h>
#include <glog/logging.h>

int multiply(int a, int b)
{
    LOG(INFO) << "multiply int result: " << a*b ;
    return a*b;
}

float multiply(float a, float b)
{
    LOG(INFO) << "multiply int result: " << a*b ;
    return a*b;
}

double multiply(double a, double b)
{
    LOG(INFO) << "multiply int result: " << a*b ;
    return a*b;
}

