#include "calc.h"

op_result *compute_1_svc(operands args, struct svc_req *rqstp)
{
    static op_result res;
    res.sum = args.a + args.b;
    res.diff = args.a - args.b;
    res.prod = args.a * args.b;
    if (args.b == 0) {
        res.div_zero = 1;
        res.quot = 0.0;
    } else {
        res.div_zero = 0;
        res.quot = (double)args.a / (double)args.b;
    }
    return &res;
}

int calcprog_1_freeresult(SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{
    (void)transp;
    (void)xdr_result;
    (void)result;
    return 1;
}
