import {Injectable} from '@angular/core';
import {Observable, of} from 'rxjs';
import {TestModel} from './model/testModel';
import {Builder} from 'builder-pattern';

@Injectable({
  providedIn: 'root'
})

export class DataService {

  dailyForecast(): Observable<TestModel[]> {
    const list = [];
    for (let i = 0; i < 10; i++) {
      list.push(Builder(TestModel)
        .name('Base ' + i)
        .x(Math.floor(Math.random() * (600 - 0 + 1)))
        .y(Math.floor(Math.random() * (600 - 0 + 1)))
        .build());
    }
    return of(list);
  }

}
