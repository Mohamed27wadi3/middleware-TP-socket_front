/* calc.x - IDL exercice 3 */

struct operands {
    int a;
    int b;
};

struct op_result {
    int sum;
    int diff;
    int prod;
    double quot;
    int div_zero; /* 1 si division impossible */
};

program CALCPROG {
    version CALCVERS {
        op_result COMPUTE(operands) = 1;
    } = 1;
} = 0x31234568;
