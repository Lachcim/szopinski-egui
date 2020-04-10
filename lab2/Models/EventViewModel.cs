using System;

namespace SzopinskiCalendar.Models {
    public class EventViewModel {
        public int Id;
        public DateTime Time;
        public string Description;

        public EventViewModel() {}
        public EventViewModel(int year, int month, int day)
        {
            try
            {
                Id = -1;
                Time = new DateTime(year, month, day, 0, 0, 0);
            }
            catch (Exception)
            {
                throw new ArgumentException("Invalid date");
            }
        }
    }
}
