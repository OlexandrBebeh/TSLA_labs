package com.company.fileio;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

public class FileInput {
    final static String FILENAME = "result.txt";

    public static void writeToFile(StringBuilder res){
        File file = new File(FILENAME);
        try {
            FileWriter fileWriter = new FileWriter(file);

            fileWriter.write(String.valueOf(res));

            fileWriter.close();
        } catch (IOException e){
            System.out.println(e.getMessage());
        }
    }
}
