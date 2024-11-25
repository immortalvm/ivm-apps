'use strict';

const { app, BrowserWindow } = require('electron');

app.whenReady().then(() => {
    const win = new BrowserWindow({
        width: 800,
        height: 800,
        autoHideMenuBar: true,
        icon: 'iVM_logo.png',
        webPreferences: {
        nodeIntegration: true,
        contextIsolation: false,
        }
    });
    win.loadFile('index.html');
});
