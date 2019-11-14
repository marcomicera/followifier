import { Injectable } from '@angular/core';
import { Observable } from 'rxjs/Observable';
import { HttpClient } from '@angular/common/http';

export interface Packet {
  name: string;
}
export interface Board {
  mac: string;
  x: string;
  y: string;
}

@Injectable()
export class ApiService {

  constructor(private http: HttpClient) {
  }
  getProva(): Observable<Packet> {
    return this.http.get<Packet>('http://localhost:8000/api');
  }
  getBoards(): Observable<Board[]> {
    return this.http.get<Board[]>('http://localhost:8000/api/boards');
  }
}
