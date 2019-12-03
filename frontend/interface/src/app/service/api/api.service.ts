import { Injectable } from '@angular/core';
import { Observable } from 'rxjs/Observable';
import { HttpClient } from '@angular/common/http';

export interface Board {
  mac: string;
  x: string;
  y: string;
}
export interface Device {
  _id: string;
  x: string;
  y: string;
}
export interface DeviceHistorical {
  _id: string;
  n: string;
}
export interface DeviceMac {
  _id: string;
}
@Injectable()
export class ApiService {

  constructor(private http: HttpClient) {
  }

  getBoards(): Observable<Board[]> {
    return this.http.get<Board[]>('http://localhost:8000/api/boards');
  }

  getDevicesNumber(): Observable<string> {
    return this.http.get<string>('http://localhost:8000/api/device/number');
  }

  getAllDevicesNumber(): Observable<Device[]> {
    return this.http.get<Device[]>('http://localhost:8000/api/devices/all');
  }
  getDevices(): Observable<Device[]> {
    return this.http.get<Device[]>('http://localhost:8000/api/devices');
  }
  getDevicesHistorical(minutes: string): Observable<DeviceHistorical[]> {
    console.log('Minute: ' + minutes);
    return this.http.get<DeviceHistorical[]>('http://localhost:8000/api/devices/historical?' + 'minutes=' + minutes);
  }
  getAllMacDevices(): Observable<DeviceMac[]> {
    return this.http.get<DeviceMac[]>('http://localhost:8000/api/devices/all');
  }
  getAllMacPosition(mac: string): Observable<Device[]> {
    return this.http.get<Device[]>('http://localhost:8000/api/devices/position?' + 'mac=' + mac);
  }
}
