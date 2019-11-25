import {ChangeDetectorRef, Component, OnInit} from '@angular/core';
import {ChartDataSets, ChartOptions, ChartType, RadialChartOptions} from 'chart.js';
import { Label } from 'ng2-charts';
import {ApiService, DeviceHistorical} from '../../service/api/api.service';


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
  public value: string;
  public selected;
  constructor(private apiService: ApiService, private changeDetectorRefs: ChangeDetectorRef) {
  }

  ngOnInit(): void {
    console.log('LOG: OnInit');
  }

  button(): void {
    console.log(this.selected);

    if (this.selected === 'hours') {
      this.value = String((parseInt(this.value, 10) * 60));
      console.log('vake ' + this.value);

      this.apiService.getDevicesHistorical(this.value).subscribe(devices => {
        this.devicesHistorical = devices;
        console.log(this.devicesHistorical);
        this.changeDetectorRefs.detectChanges();
      });
    }
    if (this.selected === 'days') {
      this.value = String((parseInt(this.value, 10) * 60 * 24));
      console.log('vake ' + this.value);

      this.apiService.getDevicesHistorical(this.value).subscribe(devices => {
        this.devicesHistorical = devices;
        console.log(this.devicesHistorical);
        this.changeDetectorRefs.detectChanges();
      });
    }
    if (this.selected === 'minutes') {
      this.value = String((parseInt(this.value, 10)));
      console.log('vake ' + this.value);

      this.apiService.getDevicesHistorical(this.value).subscribe(devices => {
        this.devicesHistorical = devices;
        console.log(this.devicesHistorical);
        this.changeDetectorRefs.detectChanges();
      });
    }
    if (this.selected === 'seconds') {
      this.value = String((parseInt(this.value, 10) / 60));
      console.log('vake ' + this.value);

      this.apiService.getDevicesHistorical(this.value).subscribe(devices => {
        this.devicesHistorical = devices;
        console.log(this.devicesHistorical);
        this.changeDetectorRefs.detectChanges();
      });
    }
  }

  }

