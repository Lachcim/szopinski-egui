using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Http;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;

namespace SzopinskiCalendar
{
    public class Startup
    {
        // This method gets called by the runtime. Use this method to add services to the container.
        // For more information on how to configure your application, visit https://go.microsoft.com/fwlink/?LinkID=398940
        public void ConfigureServices(IServiceCollection services)
        {
            services.AddControllersWithViews();
        }

        // This method gets called by the runtime. Use this method to configure the HTTP request pipeline.
        public void Configure(IApplicationBuilder app, IWebHostEnvironment env)
        {
            app.UseExceptionHandler("/error");
            app.UseStaticFiles();
            app.UseRouting();
            app.UseEndpoints(endpoints =>
            {
                endpoints.MapControllerRoute("today", "", new { controller="Calendar", action="DisplayCalendar" });
                endpoints.MapControllerRoute("month", "{year}-{month}", new { controller="Calendar", action="DisplayCalendar" });
                endpoints.MapControllerRoute("date", "{year}-{month}-{day}", new { controller="Calendar", action="DisplayDate" });
                endpoints.MapControllerRoute("event", "{year}-{month}-{day}/new", new { controller="Calendar", action="AddEvent" });
                endpoints.MapControllerRoute("event", "edit/{id}", new { controller="Calendar", action="EditEvent" });
                endpoints.MapControllerRoute("error", "error", new { controller="Calendar", action="ShowError" });
            });
        }
    }
}
