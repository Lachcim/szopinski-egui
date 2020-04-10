using System;
using Microsoft.AspNetCore.Mvc;

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
            return View();
        }

        [Route("{year:int}-{month:int}-{day:int}")]
        public string DisplayDate(int year, int month, int day)
        {
            return $"Displaying individual date: {year}-{month}-{day}";
        }

        public string AddEvent(int year, int month, int day)
        {
            return $"Adding new event to {year}-{month}-{day}";
        }

        public string EditEvent(int year, int month, int day, int index)
        {
            return $"Editing individual event: #{index} of {year}-{month}-{day}";
        }
    }
}
