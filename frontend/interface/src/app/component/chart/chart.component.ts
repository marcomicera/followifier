import {Component, ElementRef, HostBinding, OnDestroy, OnInit, ViewChild} from '@angular/core';
import {
  Chart, ChartData,
  ChartDataSets,
  ChartOptions,
  ChartPoint,
  ChartScales, ChartSize,
  ChartTitleOptions, ChartTooltipCallback, ChartTooltipItem,
  ChartTooltipOptions,
  ChartType
} from 'chart.js';
import {ActivatedRoute} from '@angular/router';
import {ApiService, Board} from '../../service/api/api.service';
import {Observable, interval, Subscription} from 'rxjs';
import {Color, Label} from "ng2-charts";

@Component({
  selector: 'app-chart',
  templateUrl: './chart.component.html',
  styleUrls: ['./chart.component.scss']
})
export class ChartComponent implements OnInit {
  // Radar
  numberDevice: number;
  private static readonly radarUpdatingFrequency = 2 * 1000; // low value for testing purposes
  private updateSubscription: Subscription;
  public scatterChartDataSet: ChartDataSets[] = [{
    label: 'Boards',
    pointRadius: 10,
    pointBackgroundColor: 'blue',
    backgroundColor: 'blue',
    hoverBackgroundColor: 'blue',
    data: [],
    hideInLegendAndTooltip: false,
  },
    {
      label: 'Devices',
      pointRadius: 10,
      pointBackgroundColor: 'red',
      backgroundColor: 'red',
      hoverBackgroundColor: 'red',
      data: []

    }];
  public scatterToolTipItem: ChartTooltipItem[] = [];
  public scatterChartData: ChartData = {
    datasets: this.scatterChartDataSet,
  }

  public scatterCallback: ChartTooltipCallback = {
    title(item: Chart.ChartTooltipItem[], data: Chart.ChartData): string | string[] {
      return ((data.datasets[item[0].datasetIndex].data[item[0].index] as ChartPoint).t as string);
    }
  };
  public scatterToolTipOptions: ChartTooltipOptions = {
    enabled: true,
    callbacks: this.scatterCallback,
  };
  public scatterChartType: ChartType = 'scatter';
  public scatterChartOptions: ChartOptions = {
    responsive: true,
    tooltips: this.scatterToolTipOptions,
  };
  public scatterChartSize: ChartSize = {
    height: 180,
    width: 180,
  };
  public lineChartData: ChartDataSets[] = [
    { data: [], label: 'Devices' },
  ];

  lineChartLabels: Label[] = [];

  lineChartOptions = {
    responsive: true,
  };

  lineChartColors: Color[] = [
    {
      borderColor: 'black',
      backgroundColor: 'rgba(255,255,0,0.28)',
    },
  ];

  private static readonly lineChartUpdatingFrequency = 5 * 1000;  // low value for testing purposes
  lineChartLegend = true;
  lineChartPlugins = [];
  lineChartType = 'line';
  constructor(private route: ActivatedRoute, private apiService: ApiService) {
  }

  ngOnInit() {
    this.updateSubscription = interval(ChartComponent.lineChartUpdatingFrequency).subscribe((val) => {
      console.log('Updating devices number line chart');
      this.apiService.getDevicesNumber().subscribe(data => {
          this.numberDevice = +data;
          this.lineChartData[0].data.push(this.numberDevice);
          this.lineChartLabels.push(String(new Date().getMinutes()));
        }
      );
    });
    this.updateSubscription = interval(ChartComponent.radarUpdatingFrequency).subscribe((val) => {
      console.log('Updating radar');
      this.scatterToolTipItem  = [];
      this.apiService.getBoards().subscribe(data => {
        this.scatterChartDataSet[0].data = [];

        data.forEach(d => {
          console.log('board: ' + d.mac);

          (this.scatterChartDataSet[0].data as ChartPoint[]).push({x: d.x, y: d.y, t: d.mac});
          (this.scatterToolTipItem.push({label: d.mac, datasetIndex: 0, index: this.scatterChartDataSet[0].data.length}));

        });
      });
      this.apiService.getDevices().subscribe(data => {
        this.scatterChartDataSet[1].data  = [];
        data.forEach(d => {
          console.log('device: ' + d._id);
          (this.scatterChartDataSet[1].data as ChartPoint[]).push({x: d.x, y: d.y, t: d._id});
          (this.scatterToolTipItem.push({label: d._id, datasetIndex: 1, index: this.scatterChartDataSet[1].data.length}));
        });
      });
    });
  }
};
