package com.company.work;

public class SequentialAnalysis{

    private static boolean check(String str, String alphabet){

        if(alphabet.length() == 0) return false;
        if(str.length() == 0) return false;
        if(str.charAt(0) != alphabet.charAt(0)) return false;

        for(int i = 1;i<str.length();i++){
            if(str.charAt(i) != alphabet.charAt(0)){
                return check(str.substring(i),alphabet.substring(1));
            }
        }
        return true;
    }

    public static boolean analysis(String str,String alphabet){
        return check(str,alphabet);
    }
}
