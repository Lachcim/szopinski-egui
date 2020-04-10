using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using Microsoft.AspNetCore.Mvc;
using SzopinskiCalendar.Models;

namespace SzopinskiCalendar.Controllers {
    public class CalendarController : Controller
    {
        public IActionResult DisplayCalendar()
        {
            int year = DateTime.Today.Year;
            int month = DateTime.Today.Month;
            
            return DisplayCalendar(year, month);
        }

        [Route("{year:int}-{month:int}")]
        public IActionResult DisplayCalendar(int year, int month) {
            CalendarViewModel data = new CalendarViewModel();
            data.Year = year;
            data.Month = month;
            data.MonthName = new CultureInfo("en-US").DateTimeFormat.MonthNames[month - 1];
            data.StartWeekday = ((int)new DateTime(year, month, 1).DayOfWeek + 6) % 7;
            data.DayCount = DateTime.DaysInMonth(year, month);
            data.Events = GetEvents(year, month);

            if (data.Events == null) {
                HttpContext.Response.StatusCode = 500;
                return Content("Error reading calendar data");
            }

            return View(data);
        }

        [Route("{year:int}-{month:int}-{day:int}")]
        public IActionResult DisplayDate(int year, int month, int day)
        {
            DateViewModel data = new DateViewModel();
            data.Year = year;
            data.Month = month;
            data.Day = day;
            
            Dictionary<int, List<EventViewModel>> events = GetEvents(year, month);
            if (events == null) {
                HttpContext.Response.StatusCode = 500;
                return Content("Error reading calendar data");
            }

            data.Events = events[day];

            return View(data);
        }

        [HttpPost]
        [Route("{year:int}-{month:int}-{day:int}")]
        public IActionResult ModifyDate(string action, int year, int month, int day, int id=0) {
            if (action == "edit")
                return RedirectToAction("EditEvent", new { id=id });
            else if (action == "delete")
                return DeleteEvent(id, year, month, day);
            else if (action == "new")
                return RedirectToAction("AddEvent", new { year=year, month=Pad(month), day=Pad(day) });
            else {
                HttpContext.Response.StatusCode = 400;
                return Content("Invalid action");
            }
        }

        public IActionResult DeleteEvent(int id, int year, int month, int day)
        {
            Dictionary<int, List<EventViewModel>> events = GetEvents();

            foreach (List<EventViewModel> list in events.Values) {
                bool found = false;
                
                foreach (EventViewModel ev in list) 
                    if (ev.Id == id) {
                        list.Remove(ev);
                        found = true;
                        break;
                    }

                if (found) break;
            }

            if (!SaveEvents(events)) {
                HttpContext.Response.StatusCode = 500;
                return Content("Error writing calendar data");
            }

            return RedirectToAction("DisplayDate", new { year=year, month=Pad(month), day=Pad(day) });
        }

        [Route("{year:int}-{month:int}-{day:int}/new")]
        public IActionResult AddEvent(int year, int month, int day)
        {
            return EditEvent(-1, year, month, day);
        }

        public IActionResult EditEvent(int id, int year=0, int month=0, int day=0)
        {
            EventViewModel data;

            if (id == -1) {
                data = new EventViewModel();
                data.Id = -1;
                data.Time = new DateTime(year, month, day, 00, 00, 00);
            }
            else {
                Dictionary<int, List<EventViewModel>> events = GetEvents();

                bool found = false;
                foreach (List<EventViewModel> list in events.Values) {
                    foreach (EventViewModel ev in list) 
                        if (ev.Id == id) {
                            data = ev;
                            found = true;
                        }

                    if (found) break;
                }

                if (!found) {
                    HttpContext.Response.StatusCode = 400;
                    return Content("No such event");
                }
            }

            return Content("Editing event " + id);
        }

        private Dictionary<int, List<EventViewModel>> GetEvents(int year=0, int month=0) {
            Dictionary<int, List<EventViewModel>> output = new Dictionary<int, List<EventViewModel>>();

            for (int i = 1; i <= 31; i++)
                output.Add(i, new List<EventViewModel>());

            if (!System.IO.File.Exists("calendar.txt")) {
                using (System.IO.File.Create("calendar.txt")) {}
                return output;
            }    

            try {
                using (StreamReader reader = System.IO.File.OpenText("calendar.txt")) {
                    while (!reader.EndOfStream) {
                        EventViewModel entry = new EventViewModel();
                        entry.Id = Convert.ToInt32(reader.ReadLine());
                        entry.Time = DateTime.ParseExact(reader.ReadLine(), "yyyy-MM-dd HH:mm", CultureInfo.InvariantCulture);
                        entry.Description = reader.ReadLine();

                        if ((entry.Time.Year == year && entry.Time.Month == month) || month == 0)
                            output[entry.Time.Day].Add(entry);
                    }
                }
            }
            catch (Exception) {
                return null;
            }
            
            for (int i = 1; i <= 31; i++)
                output[i].Sort(delegate(EventViewModel a, EventViewModel b) {
                    return a.Time.CompareTo(b.Time);
                });

            return output;
        }

        private bool SaveEvents(Dictionary<int, List<EventViewModel>> events) {
          try {
                using (StreamWriter writer = new System.IO.StreamWriter("calendar.txt")) {
                    foreach (List<EventViewModel> list in events.Values)
                        foreach (EventViewModel ev in list) {
                            writer.WriteLine(ev.Id);
                            writer.WriteLine(ev.Time.ToString("yyyy-MM-dd HH:mm"));
                            writer.WriteLine(ev.Description);
                        }
                }
            }
            catch (Exception) {
                return false;
            }

            return true;
        }

        private string Pad(int input) {
            return input.ToString().PadLeft(2, '0');
        }
    }
}
