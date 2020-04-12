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

        public string GetPrevMonth() {
            int month = Month - 1;
            int year = Year;
            if (month == 0) {
                month = 12;
                year--;
            }

            return year + "-" + month.ToString().PadLeft(2, '0');
        }
        public string GetNextMonth() {
            int month = Month + 1;
            int year = Year;
            if (month == 13) {
                month = 1;
                year++;
            }

            return year + "-" + month.ToString().PadLeft(2, '0');
        }
    }
}
