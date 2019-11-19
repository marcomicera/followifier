import { Injectable } from '@angular/core';
import { Observable } from 'rxjs/Observable';
import { HttpClient } from '@angular/common/http';

export interface Board {
  mac: string;
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
}
