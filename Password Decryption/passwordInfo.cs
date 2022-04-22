using System;

internal class Program
{
    private static void Main()
    {
        var table = new DataTable();

        using (var csvReader = new CsvReader(new StreamReader(System.IO.File.OpenRead(@"C:\Users\timca\source\repos\cpts427_hw1_Timothy_J_Cain/password1.txt")), true))
        {
            table.Load(csvReader);
        }

        passwords.password temp = new passwords.password();
        temp.password = table.Rows[1799].ToString();

        int rowsNum = table.Rows.Count;

        for(int i = 0; i<rowsNum; i++)
        {
            
            
        }

    }
}
