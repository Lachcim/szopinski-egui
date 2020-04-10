using System;
using System.Collections.Generic;

namespace SzopinskiCalendar.Models {
    public class CalendarViewModel {
        public int Year;
        public int Month;
        public string MonthName;
        public int StartWeekday;
        public int DayCount;
        public Dictionary<int,List<EventViewModel>> Events;
    }
}
