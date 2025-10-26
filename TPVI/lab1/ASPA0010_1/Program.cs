using BSTU.Results.Collection; // ��� ������� ResultsService
using Microsoft.OpenApi.Models; // ��� Swagger

//� powershell ��� ������������ �������� ����� � ������� � �����
//� ���� ������� dotnet add package BSTU.Results.Collection --source "D:\������� 5 ���\����\lab1_new\LocalNuGet"
//��� ��������

var builder = WebApplication.CreateBuilder(args);

// Add services to the container.
builder.Services.AddControllersWithViews(); // ��� MVC

// ����������� ������� ResultsService ��� Transient
builder.Services.AddTransient<IResultsService, ResultsService>(sp => new ResultsService("results.json"));

// ��������� OpenAPI (Swagger)
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen(c =>
{
    c.SwaggerDoc("v1", new OpenApiInfo { Title = "ASPA0010_1 API", Version = "v1" });
});

var app = builder.Build();

// Configure the HTTP request pipeline.
if (!app.Environment.IsDevelopment())
{
    app.UseExceptionHandler("/Home/Error");
    app.UseHsts();
}

app.UseHttpsRedirection();
app.UseStaticFiles();

app.UseRouting();

app.UseAuthorization();

// �������� Swagger ������ � ������ ����������
if (app.Environment.IsDevelopment())
{
    app.UseSwagger();
    app.UseSwaggerUI(c =>
    {
        c.SwaggerEndpoint("/swagger/v1/swagger.json", "ASPA0010_1 API v1");
    });
}

app.MapControllerRoute(
    name: "default",
    pattern: "{controller=Home}/{action=Index}/{id?}");

app.Run();