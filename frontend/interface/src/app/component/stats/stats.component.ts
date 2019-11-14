import { Component, OnInit } from '@angular/core';
import {ChartDataSets, ChartOptions, ChartType, RadialChartOptions} from 'chart.js';
import { Label } from 'ng2-charts';
import {ApiService} from '../../service/api/api.service';

@Component({
  selector: 'app-stats',
  templateUrl: './stats.component.html',
  styleUrls: [ './stats.component.scss' ]
})
export class StatsComponent implements OnInit {
  // Radar
  public variable;

  constructor(private apiService: ApiService) {

  }

  ngOnInit(): void {
    console.log('LOG: OnInit');
    this.apiService.getProva().subscribe(data => this.variable = data);
  }

}
