package com.company;

import com.company.work.SequentialAnalysis;

import java.util.Scanner;

public class Main {

    public static void main(String[] args) {

        Scanner scanner = new Scanner(System.in);

        while (true) {
            System.out.println("Hello, please enter an alphabet." +
                    "\n(print '&exit' to exit)");
            String a = scanner.nextLine();
            if(a.equals("&exit")) break;

            while (true) {
                System.out.println("Please, enter a word." +
                        "\n(print '&exit' to exit)");
                String w = scanner.nextLine();
                if(w.equals("&exit")) break;
                System.out.println(
                        SequentialAnalysis.analysis(w,a)
                                ?
                                "This word in grammar!"
                                :
                                "Word not in grammar!!!"
                );
            }
        }
    }
}
