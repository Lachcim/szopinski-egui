using System;
using System.Collections.Generic;
using System.Globalization;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Diagnostics;
using SzopinskiCalendar.Models;

namespace SzopinskiCalendar.Controllers {
    public class ApiController : Controller
    {
        public string GetMonth(int year, int month)
        {
            return "month view";
        }
        
        [HttpGet]
        [Route("api/date/{year:int}-{month:int}-{day:int}")]
        public string GetDate(int year, int month, int day)
        {
            return "date view";
        }
        
        [HttpGet]
        public string GetEvent(int id) {
            return "event view";
        }
        
        [HttpPost]
        [Route("api/date/{year:int}-{month:int}-{day:int}")]
        public string ManageEvent(int year, int month, int day) {
            return "creating event";
        }
        
        [HttpPost]
        public string ManageEvent(int id) {
            return "editing event";
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public string ShowError()
        {
            IExceptionHandlerFeature context = HttpContext.Features.Get<IExceptionHandlerFeature>();
            
            if (context == null)
                return "";

            HttpContext.Response.StatusCode = context.Error is ArgumentException ? 400 : 500;
            string status = HttpContext.Response.StatusCode == 400 ? "client error" : "server error";
            string errorType =  context.Error.GetType().Name;
            string errorMessage = context.Error.Message;

            return "error";
        }

        private string Pad(int input) {
            return input.ToString().PadLeft(2, '0');
        }
    }
}
