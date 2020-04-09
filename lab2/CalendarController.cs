using Microsoft.AspNetCore.Mvc;

namespace SzopinskiCalendar.Controllers {
    public class CalendarController : Controller
    {
        public string DisplayCalendar()
        {
            return "Displaying calendar";
        }

        public string DisplayDate()
        {
            return "Displaying individual date";
        }

        public string EditEvent()
        {
            return "Editing individual event";
        }
    }
}
