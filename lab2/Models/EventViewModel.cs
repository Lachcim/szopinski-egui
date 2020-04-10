using System;

namespace SzopinskiCalendar.Models {
    public class EventViewModel {
        public int Id;
        public DateTime Time;
        public string Description;

        public EventViewModel() {}
        public EventViewModel(int year, int month, int day, int hour, int minute)
        {
            try
            {
                Id = -1;
                Time = new DateTime(year, month, day, hour, minute, 0);
            }
            catch (Exception)
            {
                throw new ArgumentException("Invalid date");
            }
        }
    }
}
