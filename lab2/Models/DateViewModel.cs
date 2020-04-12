using System;
using System.Collections.Generic;

namespace SzopinskiCalendar.Models {
    public class DateViewModel {
        public int Year;
        public int Month;
        public int Day;
        public List<EventViewModel> Events;

        public string DateString
        {
            get
            {
                return Year
                    + "-"
                    + Month.ToString().PadLeft(2, '0')
                    + "-"
                    + Day.ToString().PadLeft(2, '0');
            }
        }
    }
}
