using System;
using System.Collections.Generic;
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
            data.Year = 1997;
            data.Month = 5;
            data.MonthName = "May";
            data.StartWeekday = 2;
            data.DayCount = 31;
            data.Events = new Dictionary<int, List<EventViewModel>>();

            EventViewModel event1 = new EventViewModel();
            event1.Id = 555;
            event1.Time = new DateTime(1997, 05, 07, 14, 00, 00);
            event1.Description = "Walk the dog";
            EventViewModel event2 = new EventViewModel();
            event2.Id = 666;
            event2.Time = new DateTime(1997, 05, 07, 15, 00, 00);
            event2.Description = "Buy new shoes";
            EventViewModel event3 = new EventViewModel();
            event3.Id = 777;
            event3.Time = new DateTime(1997, 05, 08, 14, 00, 00);
            event3.Description = "Take a shower";

            List<EventViewModel> list1 = new List<EventViewModel>();
            list1.Add(event1);
            list1.Add(event2);
            List<EventViewModel> list2 = new List<EventViewModel>();
            list2.Add(event3);

            data.Events.Add(7, list1);
            data.Events.Add(8, list2);
            
            return View(data);
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
