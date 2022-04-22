
using System;
using System.Security.Cryptography;
using System.Text;
using LumenWorks.Framework.IO.Csv;
using System.IO;
using System.Data;

namespace CryptoLib
{
    public static class encrypt
    {
        public static string toMD5(string txt)
        {
            MD5 md5 = new MD5CryptoServiceProvider();

            md5.ComputeHash(ASCIIEncoding.ASCII.GetBytes(txt));

            byte[] result = md5.Hash;

            StringBuilder eString = new StringBuilder();

            for (int i = 0; i < result.Length; i++)
            {
                eString.Append(result[i].ToString("x2"));
            }
            return eString.ToString();
        }
    }

}

public class find
{
    public static string findSalt(string input, string md5)
    {
        var table = new DataTable();

        using (var csvReader = new CsvReader(new StreamReader(System.IO.File.OpenRead(@"C:\Users\timca\source\repos\cpts427_hw1_Timothy_J_Cain/password1.txt")), true))
        {
            table.Load(csvReader);
        }

        string password = table.Rows[1][4].ToString();

        string output = CryptoLib.encrypt.toMD5(input);
        return output;
    }
}
namespace LumenWorks.Framework.IO.Csv
{
    internal class Program
    {
        private static void Main()
        {
            string input = "helloworld";
            

            Console.WriteLine(find.findSalt(input, "h"));

        }
    }
}