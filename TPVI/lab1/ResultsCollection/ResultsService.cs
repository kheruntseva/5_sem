using System;
using System.Collections.Generic;
using System.IO;
using System.Text.Json;
//"D:\проекты 5 сем\ТПВИ\lab1_new\LocalNuGet"
//Install-Package BSTU.Results.Collection -Source "D:\проекты 5 сем\ТПВИ\lab1_new\LocalNuGet"
namespace BSTU.Results.Collection
{
    public interface IResultsService
    {
        Dictionary<int, string> GetAll();
        string GetByKey(int key);
        (int Key, string Value) Add(string value);
        string Update(int key, string value);
        string Delete(int key);
    }

    public class ResultsService : IResultsService
    {
        private readonly string _filePath;
        private readonly object _lock = new object(); // Для потокобезопасности

        public ResultsService(string filePath = "results.json")
        {
            _filePath = filePath;
            if (!File.Exists(_filePath))
            {
                File.WriteAllText(_filePath, "{}"); // Пустой JSON
            }
        }

        private Dictionary<int, string> Load()
        {
            var json = File.ReadAllText(_filePath);
            return JsonSerializer.Deserialize<Dictionary<int, string>>(json) ?? new Dictionary<int, string>();
        }

        private void Save(Dictionary<int, string> data)
        {
            var json = JsonSerializer.Serialize(data);
            File.WriteAllText(_filePath, json);
        }

        public Dictionary<int, string> GetAll()
        {
            lock (_lock)
            {
                return Load();
            }
        }

        public string GetByKey(int key)
        {
            lock (_lock)
            {
                var data = Load();
                return data.TryGetValue(key, out var value) ? value : null;
            }
        }

        public (int Key, string Value) Add(string value)
        {
            if (string.IsNullOrWhiteSpace(value)) throw new ArgumentException("Value cannot be empty");

            lock (_lock)
            {
                var data = Load();
                int newKey = data.Count > 0 ? data.Keys.Max() + 1 : 1;
                data[newKey] = value;
                Save(data);
                return (newKey, value);
            }
        }

        public string Update(int key, string value)
        {
            if (string.IsNullOrWhiteSpace(value)) throw new ArgumentException("Value cannot be empty");

            lock (_lock)
            {
                var data = Load();
                if (!data.ContainsKey(key)) return null;
                data[key] = value;
                Save(data);
                return value;
            }
        }

        public string Delete(int key)
        {
            lock (_lock)
            {
                var data = Load();
                if (!data.TryGetValue(key, out var value)) return null;
                data.Remove(key);
                Save(data);
                return value;
            }
        }
    }
}