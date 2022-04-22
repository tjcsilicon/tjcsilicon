using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;



namespace cpts427_HW2_TimothyJCain
{
    class Program
    { 
        static void Main(string[] args)
        {
            decription.decrypt(@"C:\Users\timca\source\repos\cpts427_HW2_TimothyJCain\question2.txt"); //FILL IN YOUR OWN PATH
        }
    }
}

public static class decription
{
    public static void decrypt(string filePath)
    {
        var decryptKey = new Dictionary<char, char>();
        decryptKey.Add('S', 'A');
        decryptKey.Add('U', 'B');
        decryptKey.Add('N', 'C');
        decryptKey.Add('D', 'D');
        decryptKey.Add('I', 'E');
        decryptKey.Add('V', 'F');
        decryptKey.Add('E', 'G');
        decryptKey.Add('R', 'H');
        decryptKey.Add('A', 'I');
        decryptKey.Add('B', 'J');
        decryptKey.Add('C', 'K');
        decryptKey.Add('F', 'L');
        decryptKey.Add('G', 'M');
        decryptKey.Add('H', 'N');
        decryptKey.Add('J', 'O');
        decryptKey.Add('K', 'P');
        decryptKey.Add('M', 'R');
        decryptKey.Add('O', 'S');
        decryptKey.Add('P', 'T');
        decryptKey.Add('Q', 'U');
        decryptKey.Add('T', 'V');
        decryptKey.Add('W', 'W');
        decryptKey.Add('X', 'X');
        decryptKey.Add('Y', 'Y');
        decryptKey.Add('Z', 'Z');


        var alphabet = new char[26];
        var reader = new StreamReader(File.OpenRead(filePath));
        using (StreamWriter decrypted = new StreamWriter(@"C:\Users\timca\source\repos\cpts427_HW2_TimothyJCain/decrypted.txt")) //FILL IN YOUR OWN PATH!!!!
        {
            while (!reader.EndOfStream)
            {
                string line = reader.ReadLine();
                for (int i = 0; i < line.Length; i++)
                {
                    if (line[i] == '?')
                    {
                        decrypted.Write(line[i]);
                    }
                    else if (decryptKey.ContainsKey(line[i]))
                    {
                        decrypted.Write(decryptKey[line[i]]);
                    }
                    else
                    {
                        decrypted.Write(line[i]);
                    }
                }
                decrypted.Write("\n");
            }
            decrypted.Close();
        }
    }
}
