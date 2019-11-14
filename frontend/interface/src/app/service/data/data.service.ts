import {Injectable} from '@angular/core';
import {BehaviorSubject, Observable, of} from 'rxjs';
import {TestModel} from '../../model/testModel';
import {Builder} from 'builder-pattern';

@Injectable({
  providedIn: 'root'
})

export class DataService {

  public data$: BehaviorSubject<TestModel[]> = new BehaviorSubject([]);

  updateData() {
    const list = [];
    for (let i = 0; i < 10; i++) {
      list.push(Builder(TestModel)
        .name('Phone ' + i)
        .x(Math.floor(Math.random() * (800 - 0 + 1)))
        .y(Math.floor(Math.random() * (600 - 0 + 1)))
        .build());
    }
    this.data$.next(list);
  }

}
