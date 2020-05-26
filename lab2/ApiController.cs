using System;
using System.Collections.Generic;
using System.Globalization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Diagnostics;
using SzopinskiCalendar.Models;

namespace SzopinskiCalendar.Controllers {
    public class ApiController : Controller
    {
        public ActionResult GetMonth(int year, int month)
        {            
            var data = new {
                status="ok",
                year=year,
                month=month,
                days=new List<dynamic>()
            };
            
            data.days.Add(null);
            
            Dictionary<int, List<EventViewModel>> eventDictionary;
            try { eventDictionary = DataHandler.GetEventDictionary(year, month); }
            catch (Exception e) { return ShowError(e); }
            
            for (int i = 1; i <= eventDictionary.Count; i++) {
                List<dynamic> dayList = new List<dynamic>();
                
                foreach (EventViewModel ev in eventDictionary[i])
                    dayList.Add(new {
                        id=ev.Id,
                        description=ev.Description
                    });
            
                data.days.Add(dayList);
            }
            
            return Json(data);
        }
        
        [HttpGet]
        [Route("api/date/{year:int}-{month:int}-{day:int}")]
        public ActionResult GetDate(int year, int month, int day)
        {
            var data = new {
                status="ok",
                year=year,
                month=month,
                day=day,
                events=new List<dynamic>()
            };
            
            List<EventViewModel> eventList;
            try { eventList = DataHandler.GetEvents(new DateTime(year, month, day)); }
            catch (Exception e) { return ShowError(e); }
            
            foreach (EventViewModel ev in eventList)
                data.events.Add(new {
                    id=ev.Id,
                    time=((DateTimeOffset)ev.Time).ToUnixTimeSeconds() * 1000,
                    description=ev.Description
                });
            
            return Json(data);
        }
        
        [HttpGet]
        public ActionResult GetEvent(int id) {
            EventViewModel ev;
            try { ev = DataHandler.GetEvent(id); }
            catch (Exception e) { return ShowError(e); }
            
            var data = new {
                status="ok",
                id=ev.Id,
                time=((DateTimeOffset)ev.Time).ToUnixTimeSeconds() * 1000,
                description=ev.Description
            };
            
            return Json(data);
        }
        
        [HttpPost]
        [Route("api/date/{year:int}-{month:int}-{day:int}")]
        public ActionResult ManageEvent(string time, string description, int year, int month, int day) {
            int hour, minute;
            
            try {
                hour = Convert.ToInt32(time.Substring(0, 2));
                minute = Convert.ToInt32(time.Substring(3, 2));
            }
            catch (Exception) {
                throw new ArgumentException("Invalid time.");
            }
            
            EventViewModel newEvent = new EventViewModel(year, month, day, hour, minute);
            newEvent.Description = description ?? "";
            
            int newEventId;
            try { newEventId = DataHandler.AddEvent(newEvent); }
            catch (Exception e) { return ShowError(e); }
            
            return Json(new {
                status="ok",
                id=newEventId
            });
        }
        
        [HttpPost]
        public ActionResult ManageEvent(int id, string time, string description) {
            int hour, minute;
            
            try {
                hour = Convert.ToInt32(time.Substring(0, 2));
                minute = Convert.ToInt32(time.Substring(3, 2));
            }
            catch (Exception) {
                throw new ArgumentException("Invalid time.");
            }
            
            try { DataHandler.UpdateEvent(id, hour, minute, description ?? ""); }
            catch (Exception e) { return ShowError(e); }
            
            return Json(new {status="ok"});
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public ActionResult ShowError(Exception exception)
        {
            HttpContext.Response.StatusCode = exception is ArgumentException ? 400 : 500;
            string status = HttpContext.Response.StatusCode == 400 ? "client error" : "server error";
            string errorType =  exception.GetType().Name;
            string errorMessage = exception.Message;

            return Json(new {status=status, error_type=errorType, error_message=errorMessage});
        }
    }
}
