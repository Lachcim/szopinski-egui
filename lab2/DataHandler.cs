using System;
using System.Linq;
using System.IO;
using System.Collections.Generic;
using System.Globalization;
using SzopinskiCalendar.Models;

namespace SzopinskiCalendar
{
    public static class DataHandler
    {
        public static List<EventViewModel> GetEvents()
        {
            List<EventViewModel> output = new List<EventViewModel>();
            
            if (!System.IO.File.Exists("calendar.txt"))
            {
                using (System.IO.File.Create("calendar.txt")) {}
                return output;
            }
            
            try
            {
                using (StreamReader reader = System.IO.File.OpenText("calendar.txt"))
                {
                    while (!reader.EndOfStream)
                    {
                        EventViewModel entry = new EventViewModel();
                        entry.Id = Convert.ToInt32(reader.ReadLine());
                        entry.Time = DateTime.ParseExact(reader.ReadLine(), "yyyy-MM-dd HH:mm", CultureInfo.InvariantCulture);
                        entry.Description = reader.ReadLine();

                        output.Add(entry);
                    }
                }
            }
            catch (Exception) {
                throw new IOException("Error reading calendar data");
            }

            output.Sort(delegate(EventViewModel a, EventViewModel b)
            {
                return a.Time.CompareTo(b.Time);
            });

            return output;
        }

        public static List<EventViewModel> GetEvents(int year, int month, int day)
        {
            return GetEvents().Where(ev => ev.Time.Year == year && ev.Time.Month == month && ev.Time.Day == day).ToList();
        }

        public static EventViewModel GetEvent(int id)
        {
            foreach (EventViewModel ev in GetEvents())
                if (ev.Id == id)
                    return ev;

            throw new ArgumentException("No event with the given id");
        }

        public static Dictionary<int, List<EventViewModel>> GetEventDictionary(int year, int month)
        {
            Dictionary<int, List<EventViewModel>> output = new Dictionary<int, List<EventViewModel>>();

            for (int i = 1; i <= 31; i++)
                output.Add(i, new List<EventViewModel>());

            foreach (EventViewModel ev in GetEvents())
                if (ev.Time.Year == year && ev.Time.Month == month)
                    output[ev.Time.Day].Add(ev);

            return output;
        }

        public static void DeleteEvent(int id) {
            List<EventViewModel> events = GetEvents();

            foreach (EventViewModel ev in events)
                if (ev.Id == id)
                {
                    events.Remove(ev);
                    return;
                }

            throw new ArgumentException("No event with the given id");
        }

        public static void SaveEvents(List<EventViewModel> events)
        {
            try
            {
                using (StreamWriter writer = new System.IO.StreamWriter("calendar.txt"))
                {
                    foreach (EventViewModel ev in events) {
                        writer.WriteLine(ev.Id);
                        writer.WriteLine(ev.Time.ToString("yyyy-MM-dd HH:mm"));
                        writer.WriteLine(ev.Description);
                    }
                }
            }
            catch (Exception)
            {
                throw new IOException("Error writing calendar data");
            }
        }
    }
}
