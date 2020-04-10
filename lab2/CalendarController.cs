using System;
using System.Collections.Generic;
using System.Globalization;
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
        public IActionResult DisplayCalendar(int year, int month)
        {
            CalendarViewModel data = new CalendarViewModel();
            data.Year = year;
            data.Month = month;
            data.MonthName = new CultureInfo("en-US").DateTimeFormat.MonthNames[month - 1];
            data.StartWeekday = ((int)new DateTime(year, month, 1).DayOfWeek + 6) % 7;
            data.DayCount = DateTime.DaysInMonth(year, month);
            data.Events = DataHandler.GetEventDictionary(year, month);

            return View(data);
        }

        [Route("{year:int}-{month:int}-{day:int}")]
        public IActionResult DisplayDate(int year, int month, int day)
        {
            DateViewModel data = new DateViewModel();
            data.Year = year;
            data.Month = month;
            data.Day = day;
            data.Events = DataHandler.GetEvents(year, month, day);

            return View(data);
        }

        [HttpPost]
        [Route("{year:int}-{month:int}-{day:int}")]
        public IActionResult ModifyDate(string action, int year, int month, int day, int id=0)
        {
            if (action == "edit")
                return RedirectToAction("EditEvent", new { id=id });
            else if (action == "delete")
            {
                DataHandler.DeleteEvent(id);
                return RedirectToAction("DisplayDate", new { year=year, month=Pad(month), day=Pad(day) });
            }
            else if (action == "new")
                return RedirectToAction("AddEvent", new { year=year, month=Pad(month), day=Pad(day) });
            else
                throw new ArgumentException("Invalid action");
        }

        [Route("{year:int}-{month:int}-{day:int}/new")]
        public IActionResult AddEvent(int year, int month, int day)
        {
            return EditEvent(-1, year, month, day);
        }

        [Route("edit/{id:int}")]
        public IActionResult EditEvent(int id, int year=0, int month=0, int day=0)
        {
            EventViewModel data = null;

            if (id != -1)
                data = DataHandler.GetEvent(id);
            else
                data = new EventViewModel(year, month, day);

            return View("EditEvent", data);
        }

        [HttpPost]
        [Route("edit/{id:int}")]
        [Route("{year:int}-{month:int}-{day:int}/new")]
        public IActionResult ApplyChanges(int id, string time, string description, int year=0, int month=0, int day=0) {
            return Content("changes applied to " + id + ", time: " + time + ", desc: " + description + ", date: " + year + "-" + month + "-" + day);
        }

        private string Pad(int input) {
            return input.ToString().PadLeft(2, '0');
        }
    }
}
