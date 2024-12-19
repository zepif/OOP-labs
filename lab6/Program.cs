using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

public class Phone
{
    public string CountryCode { get; set; }
    public string OperatorCode { get; set; }
    public string Number { get; set; }

    public Phone() { }

    public Phone(string countryCode, string operatorCode, string number)
    {
        CountryCode = countryCode;
        OperatorCode = operatorCode;
        Number = number;
    }

    public override string ToString()
    {
        return $"+{CountryCode}({OperatorCode}){Number}";
    }
}

public class Email
{
    public string Username { get; set; }
    public string Domain { get; set; }

    public Email() { }

    public Email(string username, string domain)
    {
        Username = username;
        Domain = domain;
    }

    public override string ToString()
    {
        return $"{Username}@{Domain}";
    }
}

public class ContactContainer : IEnumerable
{
    private List<object> items;

    public ContactContainer()
    {
        items = new List<object>();
    }

    public void Add(object item)
    {
        items.Add(item);
    }

    public void Sort()
    {
        items = items.OrderBy(i => i.ToString()).ToList();
    }

    public IEnumerator GetEnumerator()
    {
        return items.GetEnumerator();
    }
}

public class Program
{
    public static void Main(string[] args)
    {
        ContactContainer container = new ContactContainer();

        container.Add(new Phone("1", "800", "1234567"));
        container.Add(new Phone("44", "20", "123456"));
        container.Add(new Phone("380", "67", "1234567"));

        container.Add(new Email("john.doe", "example.com"));
        container.Add(new Email("alice", "mail.com"));
        container.Add(new Email("bob", "service.org"));

        Console.WriteLine("До сортування:");
        foreach (var item in container)
        {
            Console.WriteLine(item.ToString());
        }

        container.Sort();

        Console.WriteLine("\nПісля сортування:");
        foreach (var item in container)
        {
            Console.WriteLine(item.ToString());
        }
    }
}

