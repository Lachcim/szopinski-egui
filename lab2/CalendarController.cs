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
                return DeleteEvent(id);
            else if (action == "new")
                return RedirectToAction("AddEvent", new { year=year, month=Pad(month), day=Pad(day) });
            else {
                HttpContext.Response.StatusCode = 400;
                return Content("Invalid action");
            }
        }

        public IActionResult DeleteEvent(int id)
        {
            return Content("Deleting " + id);
        }

        [Route("{year:int}-{month:int}-{day:int}/new")]
        public string AddEvent(int year, int month, int day)
        {
            return $"Adding new event to {year}-{month}-{day}";
        }

        public string EditEvent(int id, bool adding=false)
        {
            return $"Editing individual event {id}";
        }

        private Dictionary<int, List<EventViewModel>> GetEvents(int year, int month) {
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

                        if (entry.Time.Year == year && entry.Time.Month == month)
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

        private string Pad(int input) {
            return input.ToString().PadLeft(2, '0');
        }
    }
}
