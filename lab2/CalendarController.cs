using System;
using System.Collections.Generic;
using System.Globalization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Diagnostics;
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
            data.StartWeekday = ((int)new DateTime(year, month, 1).DayOfWeek + 6) % 7;
            data.MonthName = new CultureInfo("en-US").DateTimeFormat.MonthNames[month - 1];
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
            data.Events = DataHandler.GetEvents(new DateTime(year, month, day));

            return View(data);
        }

        [HttpPost]
        [Route("{year:int}-{month:int}-{day:int}")]
        public IActionResult DeleteEvent(string action, int year, int month, int day, int id=0)
        {
            if (action != "delete")
                throw new ArgumentException("Invalid action.");
            
            DataHandler.DeleteEvent(id);
            return RedirectToAction("DisplayDate", new { year=year, month=Pad(month), day=Pad(day) });
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
                data = new EventViewModel(year, month, day, 12, 0);

            return View("EditEvent", data);
        }

        [HttpPost]
        [Route("edit/{id:int}")]
        [Route("{year:int}-{month:int}-{day:int}/new")]
        public IActionResult ApplyChanges(int id, string time, string description, int year=0, int month=0, int day=0)
        {
            int hour, minute;
            
            try {
                hour = Convert.ToInt32(time.Substring(0, 2));
                minute = Convert.ToInt32(time.Substring(3, 2));
            }
            catch (Exception) {
                throw new ArgumentException("Invalid time.");
            }
            
            if (id != -1)
            {
                DateTime eventTime = DataHandler.UpdateEvent(id, hour, minute, description);
                return RedirectToAction("DisplayDate", new { year=eventTime.Year, month=Pad(eventTime.Month), day=Pad(eventTime.Day) });
            }
            else
            {
                EventViewModel newEvent = new EventViewModel(year, month, day, hour, minute);
                newEvent.Description = description;
                
                DataHandler.AddEvent(newEvent);
                return RedirectToAction("DisplayDate", new { year=year, month=Pad(month), day=Pad(day) });
            }
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult ShowError()
        {
            IExceptionHandlerFeature context = HttpContext.Features.Get<IExceptionHandlerFeature>();
            
            if (context == null)
                return RedirectToAction("DisplayCalendar");

            HttpContext.Response.StatusCode = context.Error is ArgumentException ? 400 : 500;
            ViewBag.ErrorType = context.Error.GetType().Name;
            ViewBag.ErrorMessage = context.Error.Message;

            return View();
        }

        private string Pad(int input) {
            return input.ToString().PadLeft(2, '0');
        }
    }
}
