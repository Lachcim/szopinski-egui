# szopinski-egui
My implementation of the EGUI project for Mr Myrcha, peace be upon Him.

## The task
The goal of the project is to create a simple calendar application capable of storing, displaying and managing events scheduled for a given day. The task is to be implemented using three different technologies.

## Setup

### Lab 1: Qt
```bash
git clone https://github.com/Lachcim/szopinski-egui
cd szopinski-egui/lab1
qmake
make
cd bin
./szopinski-egui
```
### Lab 2: ASP.NET MVC

```bash
git clone https://github.com/Lachcim/szopinski-egui
cd szopinski-egui/lab2
dotnet build
dotnet run
```
Kestrel will be listening to all hostnames on port 5000.

### Lab 3: React

```bash
git clone https://github.com/Lachcim/szopinski-egui
cd szopinski-egui/lab3
npm install
npm start
```
Running the React app requires launching **both** Webpack Development Server **and** Kestrel from lab 2 (as an API endpoint). WDS will be listening on port 8080.
