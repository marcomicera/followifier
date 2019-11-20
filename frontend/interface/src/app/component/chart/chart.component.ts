import {Component, ElementRef, HostBinding, OnDestroy, OnInit, ViewChild} from '@angular/core';
import {Chart, ChartDataSets, ChartOptions, ChartPoint, ChartScales, ChartTitleOptions, ChartType} from 'chart.js';
import {ActivatedRoute} from '@angular/router';
import {ApiService, Board} from '../../service/api/api.service';
import {Observable, interval, Subscription} from 'rxjs';

@Component({
  selector: 'app-chart',
  templateUrl: './chart.component.html',
  styleUrls: ['./chart.component.scss']
})
export class ChartComponent implements OnInit {
  // Radar
  title: ChartScales;
  numberDevice: number;
  private updateSubscription: Subscription;
  public scatterChartOptions: ChartOptions = {
    responsive: true,
  };

  public scatterChartData: ChartDataSets[] = [{
    label: 'Boards',
    pointRadius: 10,
    pointBackgroundColor: 'blue',
    backgroundColor: 'blue',
    hoverBackgroundColor: 'blue',
    data: [{}],
  },
    {
      label: 'Devices',
      pointRadius: 10,
      pointBackgroundColor: 'red',
      backgroundColor: 'red',
      hoverBackgroundColor: 'red',
      data: [{}]

    }];
  public scatterChartType: ChartType = 'scatter';


  constructor(private route: ActivatedRoute, private apiService: ApiService) {
  }

  ngOnInit() {
    this.updateSubscription = interval(10000).subscribe((val) => {
      console.log('LOG: OnInit');
      this.apiService.getDevicesNumber().subscribe(data => {
          this.numberDevice = +data;
        }
      );
      this.apiService.getBoards().subscribe(data => {
        (this.scatterChartData[0].data as ChartPoint[]) = [];
        data.forEach(d => {
          console.log('board: x:' + d.x + ' y ' + d.y + ' t ' + d.mac);
          (this.scatterChartData[0].data as ChartPoint[]).push({x: d.x, y: d.y});

        });
      });
      this.apiService.getDevices().subscribe(data => {
        (this.scatterChartData[1].data as ChartPoint[]) = [];
        data.forEach(d => {
          console.log('devices: x:' + d.x + ' y ' + d.y + ' t ' + d.mac);
          (this.scatterChartData[1].data as ChartPoint[]).push({x: d.x, y: d.y});

        });
      });
    });
  }
}
