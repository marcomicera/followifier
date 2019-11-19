import {Component, ElementRef, HostBinding, OnDestroy, OnInit, ViewChild} from '@angular/core';
import {Chart, ChartDataSets, ChartOptions, ChartPoint, ChartScales, ChartTitleOptions, ChartType} from 'chart.js';
import {ActivatedRoute} from '@angular/router';
import {ApiService, Board} from '../../service/api/api.service';

@Component({
  selector: 'app-chart',
  templateUrl: './chart.component.html',
  styleUrls: ['./chart.component.scss']
})
export class ChartComponent implements OnInit {
  // Radar
  title: ChartScales;
  public scatterChartOptions: ChartOptions = {
    responsive: true,
  };

  public scatterChartData: ChartDataSets[] = [{
    label: 'Boards',
    pointRadius: 10,
    pointBackgroundColor: 'blue',
    backgroundColor: 'blue',
    hoverBackgroundColor: 'blue',
    data: [{x: 3, y: 0}],

  }]
  public scatterChartType: ChartType = 'scatter';


  constructor(private route: ActivatedRoute, private apiService: ApiService) {
  }

  ngOnInit() {
    console.log('LOG: OnInit');
    this.apiService.getDevicesNumber().subscribe(data => {
      console.log('Numero di messagi' + data);
    });
    this.apiService.getBoards().subscribe(data => {

      data.forEach(d => {
        console.log('board: x:' + d.x + ' y ' + d.y + ' t ' + d.mac);
        (this.scatterChartData[1].data as ChartPoint[]).push({x: d.x, y: d.y});

      });
    });
  }
}
