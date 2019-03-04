const electron = require('electron');
const app = electron.app;
const BrowserWindow = electron.BrowserWindow;
const ipcMain = electron.ipcMain;

const path = require('path');
const isDev = require('electron-is-dev');

//const quesync = require('../backend/bin/win32-x64-64/backend.node');

// Set the main window as a global var
let mainWindow;

// Create a new backend client
//var client = new quesync.Client();

function createWindow() {
  // Create a new browser window with a fixed initial size
  mainWindow = new BrowserWindow({width: 900, height: 680});

  // Load the dev url if electron ran on dev or load the static html file when electron is running in production
  mainWindow.loadURL(isDev ? 'http://localhost:3000' : `file://${path.join(__dirname, '../build/index.html')}`);

  // On the close event release the window from the memory
  mainWindow.on('closed', () => mainWindow = null);
}

// Listen to a login event
ipcMain.on('client-connect', (event, serverIP) => {
  try {
    // Try to connect to the server
    client.connect(serverIP)

    // Save the client as a global var
    global.client = client;

    // Send confirmation
    event.returnValue = { error: 0 }
  } catch (ex)
  {
    // Send the error to the client
    event.returnValue = { error: ex.message }
  }
})

// When the electron app is ready, create the browser window
app.on('ready', createWindow);

// If all the windows are closed, on windows, quit app
app.on('window-all-closed', () => {
  // Shut down app on windows
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

// If the app activated and the main window isn't created, create the window
app.on('activate', () => {
  // If the main window is null, create it
  if (mainWindow === null) {
    createWindow();
  }
});