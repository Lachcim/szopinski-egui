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
                throw new IOException("Error reading calendar data.");
            }

            output.Sort(delegate(EventViewModel a, EventViewModel b)
            {
                return a.Time.CompareTo(b.Time);
            });

            return output;
        }

        public static List<EventViewModel> GetEvents(DateTime date)
        {
            return GetEvents().Where(ev => ev.Time.Date == date.Date).ToList();
        }

        public static EventViewModel GetEvent(int id)
        {
            foreach (EventViewModel ev in GetEvents())
                if (ev.Id == id)
                    return ev;

            throw new ArgumentException("No event with the given id.");
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
                    SaveEvents(events);
                    return;
                }

            throw new ArgumentException("No event with the given id.");
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
                        writer.WriteLine(ev.Description.Replace("\r", "").Replace('\n', ' '));
                    }
                }
            }
            catch (Exception)
            {
                throw new IOException("Error writing calendar data.");
            }
        }

        public static DateTime UpdateEvent(int id, int hour, int minute, string description)
        {
            List<EventViewModel> events = GetEvents();

            for (int i = 0; i < events.Count; i++)
                if (events[i].Id == id)
                {
                    int year = events[i].Time.Year;
                    int month = events[i].Time.Month;
                    int day = events[i].Time.Day;
                    
                    events[i].Time = new DateTime(year, month, day, hour, minute, 0);
                    events[i].Description = description.Replace("\r", "").Replace('\n', ' ');

                    SaveEvents(events);
                    return events[i].Time;
                }

            throw new ArgumentException("No event with the given id.");
        }

        public static int AddEvent(EventViewModel newEvent)
        {
            List<EventViewModel> events = GetEvents();
            int maxId = -1;

            foreach (EventViewModel ev in events)
                if (ev.Id > maxId)
                    maxId = ev.Id;

            newEvent.Id = maxId + 1;
            events.Add(newEvent);

            SaveEvents(events);
            return newEvent.Id;
        }
    }
}
