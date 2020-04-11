using System;

namespace SzopinskiCalendar.Models {
    public class EventViewModel {
        public int Id = -1;
        public DateTime Time;
        public string Description;

        public EventViewModel() {}
        public EventViewModel(int year, int month, int day, int hour, int minute)
        {
            Time = new DateTime(year, month, day, hour, minute, 0);
        }
    }
}
