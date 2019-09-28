import {Component, ElementRef, HostBinding, OnDestroy, OnInit, ViewChild} from '@angular/core';
import {Chart, ChartDataSets, ChartOptions, ChartType} from 'chart.js';
import { ActivatedRoute } from '@angular/router';
import {log} from 'util';
import {DataService} from '../../service/data/data.service';
import {TestModel} from '../../model/testModel';
import {any} from 'codelyzer/util/function';
import {Subject} from 'rxjs';

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

  public scatterChartData: ChartDataSets[] = [
    {
      data: [
        { x: 1, y: 1 },
        { x: 2, y: 3 },
        { x: 3, y: -2 },
        { x: 4, y: 4 },
        { x: 5, y: -3, r: 20 },
      ],
      label: 'Series A',
      pointRadius: 10,
    },
  ];
  public scatterChartType: ChartType = 'scatter';


  constructor(private route: ActivatedRoute) { }
  ngOnInit() {
  }

}
