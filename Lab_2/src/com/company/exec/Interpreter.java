package com.company.exec;
import com.company.fileio.FileInput;
import java.util.HashMap;
import java.util.Map;
import java.util.Stack;
public class Interpreter {
    static Map<String, Integer> prior = new HashMap<>();
    static StringBuilder res = new StringBuilder();
    static {
        prior.put("*",2);
        prior.put("+",1);
        prior.put("-",1);
        prior.put("==",0);
        prior.put("!=",0);
        prior.put("<",0);
        prior.put(":=",-1);
        prior.put("(",-1);
        prior.put(")",-1);
    }
    private static boolean isOperator(String str){
        return prior.containsKey(str);
    }
    private static boolean precedence(String oper1, String oper2){
        if(oper1.equals("(") || oper2.equals(")")) return false;
        if(oper2.equals("(")) return false;
        return prior.get(oper1) < prior.get(oper2);
    }
    public static void toPostfix(String str){
        String temp = str.replaceAll("\\s","");
        String[] infix = temp.split("");
        StringBuilder postfix = new StringBuilder("\n");
        Stack<String> operatorStack = new Stack<>();
        for(int i = 0; i<infix.length;i++){
            if(!isOperator( infix[i])){
                if(i+1<infix.length && isOperator( infix[i] + infix[i+1])){
                    while (!operatorStack.empty()){
                        if(precedence(operatorStack.peek(),infix[i]+ infix[i+1])) break;
                        postfix.append(operatorStack.pop());
                    }
                    postfix.append(" ");
                    operatorStack.push(infix[i] + infix[i+1]);
                    i++;
                }else {
                    postfix.append(infix[i]);
                }
            } else {
                while (!operatorStack.empty()){
                    if(precedence(operatorStack.peek(),infix[i])) break;
                    if(operatorStack.peek().equals("(") || operatorStack.peek().equals(")")) operatorStack.pop();
                    else postfix.append(operatorStack.pop());
                }
                if(!infix[i].equals("(") && !infix[i].equals(")"))
                    postfix.append(" ");
                operatorStack.push(infix[i]);
            }
        }
        while (!operatorStack.empty()){
            if(operatorStack.peek().equals("(") || operatorStack.peek().equals(")")) operatorStack.pop();
            else postfix.append(operatorStack.pop());
        }
        res.append(postfix);
    }
    public static void write(){
        FileInput.writeToFile(res);
        res = new StringBuilder();
    }
}
