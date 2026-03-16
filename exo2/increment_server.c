#include "increment.h"

int *increment_1_svc(int arg, struct svc_req *rqstp)
{
    static int result;
    result = arg + 1;
    return &result;
}

int incrprog_1_freeresult(SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{
    (void)transp;
    (void)xdr_result;
    (void)result;
    return 1;
}
