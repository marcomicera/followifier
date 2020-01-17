import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';
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
export interface Point {
  _id: string;
  x: string;
  y: string;
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
  getDevices(): Observable<Device[]> {
    return this.http.get<Device[]>('http://localhost:8000/api/devices');
  }
  getDevicesHistorical(start: string, end: string): Observable<DeviceHistorical[]> {
    return this.http.get<DeviceHistorical[]>('http://localhost:8000/api/devices/historical?' + 'start=' + start + '&end=' + end);
  }
  getDevicesHistoricalIntervalls(start: string, end: string, mac: string): Observable<DeviceHistorical[]> {
    return this.http.get<DeviceHistorical[]>('http://localhost:8000/api/device/intervalls/?' + 'start=' + start + '&end=' + end + '&mac=' + mac);
  }
  getAllMacDevices(): Observable<DeviceMac[]> {
    return this.http.get<DeviceMac[]>('http://localhost:8000/api/devices/all');
  }
  getAllMacPosition(mac: string): Observable<Device[]> {
    return this.http.get<Device[]>('http://localhost:8000/api/devices/position?' + 'mac=' + mac);
  }
  getRoomCoordinate(): Observable<Point[]> {
    return this.http.get<Point[]>('http://localhost:8000/api/room');
  }
}
