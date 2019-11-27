import {ChangeDetectorRef, Component, OnInit} from '@angular/core';
import {
  Chart,
  ChartData,
  ChartDataSets,
  ChartOptions, ChartPoint, ChartSize,
  ChartTooltipCallback,
  ChartTooltipItem, ChartTooltipOptions,
  ChartType,
  RadialChartOptions
} from 'chart.js';
import { Label } from 'ng2-charts';
import {ApiService, DeviceHistorical, DeviceMac} from '../../service/api/api.service';


@Component({
  selector: 'app-stats',
  templateUrl: './stats.component.html',
  styleUrls: [ './stats.component.scss' ]
})
export class StatsComponent implements OnInit {
  // Radar
  public devicesHistorical: DeviceHistorical[];
  displayedColumns: string[] = ['mac', 'appearance'];
  timeInterval: string[] = ['days', 'hours', 'minutes', 'seconds'];
  public macList: string[];
  public value: string;
  public selected;
  public macSelected: string;
  public scatterChartDataSet: ChartDataSets[] = [
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
  constructor(private apiService: ApiService, private changeDetectorRefs: ChangeDetectorRef) {
  }

  ngOnInit(): void {
    console.log('LOG: OnInit');
    this.apiService.getAllMacDevices().subscribe( mac => {
      this.macList = [];
      mac.forEach( m => { console.log(m._id); this.macList.push(m._id); });
    });
  }

  button(): void {
    console.log(this.selected);

    if (this.selected === 'hours') {
      this.apiService.getDevicesHistorical(String((parseInt(this.value, 10) * 60))).subscribe(devices => {
        this.devicesHistorical = devices;
        console.log(this.devicesHistorical);
        this.changeDetectorRefs.detectChanges();
      });
    }
    if (this.selected === 'days') {
      this.apiService.getDevicesHistorical(String((parseInt(this.value, 10) * 60 * 24))).subscribe(devices => {
        this.devicesHistorical = devices;
        console.log(this.devicesHistorical);
        this.changeDetectorRefs.detectChanges();
      });
    }
    if (this.selected === 'minutes') {
      this.apiService.getDevicesHistorical(String((parseInt(this.value, 10)))).subscribe(devices => {
        this.devicesHistorical = devices;
        console.log(this.devicesHistorical);
        this.changeDetectorRefs.detectChanges();
      });
    }
    if (this.selected === 'seconds') {
      this.apiService.getDevicesHistorical(String((parseInt(this.value, 10) / 60))).subscribe(devices => {
        this.devicesHistorical = devices;
        console.log(this.devicesHistorical);
        this.changeDetectorRefs.detectChanges();
      });
    }
  }
  buttonMac(): void {
    this.apiService.getAllMacPosition(this.macSelected).subscribe(position => {
      position.forEach(p => {
      (this.scatterChartDataSet[0].data as ChartPoint[]).push({x: p.x ,y: p.y, t: p._id});
      })

      console.log('position: ' + position);
    });
  }
}

