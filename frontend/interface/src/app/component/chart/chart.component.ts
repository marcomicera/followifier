import {Component, ElementRef, HostBinding, OnDestroy, OnInit, ViewChild} from '@angular/core';
import {Chart, ChartDataSets, ChartOptions, ChartPoint, ChartType} from 'chart.js';
import { ActivatedRoute } from '@angular/router';
import {log} from 'util';
import {DataService} from '../../service/data/data.service';
import {TestModel} from '../../model/testModel';
import {any} from 'codelyzer/util/function';
import {Subject} from 'rxjs';
import {ApiService, Board} from '../../service/api/api.service';

@Component({
  selector: 'app-chart',
  templateUrl: './chart.component.html',
  styleUrls: ['./chart.component.scss']
})
export class ChartComponent implements OnInit {
  // Radar
  public scatterChartOptions: ChartOptions = {
    responsive: true,
  };
  public point: ChartPoint;

  public scatterChartData: ChartDataSets[] = [{
    data: [{}],
    label: 'Boards',
    pointRadius: 10,
  }];
  public scatterChartType: ChartType = 'scatter';


  constructor(private route: ActivatedRoute, private apiService: ApiService) {
  }

  ngOnInit() {
    console.log('LOG: OnInit');
    this.apiService.getBoards().subscribe(data => {
      data.forEach(d => {
        console.error('board: x:' + d.x + ' y ' + d.y + ' t ' + d.mac );
        this.scatterChartData[0].data.push({ x: d.x, y: d.y, t: d.mac });
        console.error( this.scatterChartData[0]);
      });
    });
  }
}
