package com.company;

import com.company.exec.Interpreter;

public class Main {

    public static void main(String[] args) {

        Interpreter.toPostfix("A+B*C");
        Interpreter.toPostfix("A-B*C");
        Interpreter.toPostfix("A-B+C+D-G");
        Interpreter.toPostfix("(A+B)*C");
        Interpreter.toPostfix("((A+B)+G)*C");
        Interpreter.toPostfix("((A+B)+G)<C");
        Interpreter.toPostfix("A := 34 + C == D");
        Interpreter.toPostfix("A := 34 - 12 * C != D");
        Interpreter.write();

    }
}
