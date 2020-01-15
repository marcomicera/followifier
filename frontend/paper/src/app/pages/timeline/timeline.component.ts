import {ChangeDetectorRef, Component, OnInit} from '@angular/core';
import { IgxSliderModule } from 'igniteui-angular';
import {MatSlider} from '@angular/material/slider';

import {
    Chart,
    ChartDataSets,
    ChartOptions,
    ChartPoint,
    ChartTooltipCallback,
    ChartTooltipOptions,
    ChartType
} from "chart.js";
import {ApiService, Device} from '../../service/api/api.service';
import * as moment from 'moment'

@Component({
    selector: 'timeline-cmp',
    moduleId: module.id,
    templateUrl: 'timeline.component.html',
    styleUrls:['timeline.component.css']
})

export class TimelineComponent implements OnInit {

    public canvas: any;
    public data: string;
    public ctx;
    public index: number;
    public min: number;
    public max: number;
    public macList: string[];
    private deviceMac: string;
    private positionDevice: Device[];
    public timelineChart;
    public scatterChartDataSet: ChartDataSets[] = [{
        label: 'Devices',
        pointRadius: 10,
        pointBackgroundColor: 'red',
        backgroundColor: 'red',
        hoverBackgroundColor: 'red',
        data: []
    }];

    public scatterCallback: ChartTooltipCallback = {
        title(item: Chart.ChartTooltipItem[], data: Chart.ChartData): string | string[] {
            let ts = [] as string[];
            item.forEach(i => {
                const unixtime = ((data.datasets[i.datasetIndex].data[i.index] as ChartPoint).t) as number;
                ts.push(moment.unix(unixtime).format('DD/MM/YY HH:mm:ss'));
            });
            return ts;
        }
    };
    public scatterToolTipOptions: ChartTooltipOptions = {
        enabled: true,
        callbacks: this.scatterCallback
    };
    public scatterChartType: ChartType = 'scatter';
    public scatterChartOptions: ChartOptions = {
        responsive: true,
        legend: {
            display: false
        },
        // scales: {
        //     xAxes: [{
        //         ticks: {
        //             min: 0,
        //             max: 600 // FIXME room dimension
        //         }
        //     }],
        //     yAxes: [{
        //         ticks: {
        //             min: 0,
        //             max: 600  // FIXME room dimension
        //         }
        //     }]
        // },
        tooltips: this.scatterToolTipOptions,
    };

    constructor(private apiService: ApiService, private changeDetectorRefs: ChangeDetectorRef) {
    }

    getTimeline(): void {
        this.apiService.getAllMacPosition(this.deviceMac).subscribe(positions => {
            console.log(`Device ${this.deviceMac} positions:`);
            console.dir(positions);
            console.log(positions.length);
            this.scatterChartDataSet[0].data = [];
            this.positionDevice = positions;
            this.max = this.positionDevice.length - 1;
            this.data = moment.unix(+this.positionDevice[0]._id).format('DD/MM/YY HH:mm:ss');
            (this.scatterChartDataSet[0].data as ChartPoint[]).push({x: this.positionDevice[0].x ,y: this.positionDevice[0].y, t: this.positionDevice[0]._id});
        });
    }

    ngOnInit() {
     this.min = 0;
     this.max = 0;
        // Retrieving all devices
        this.apiService.getAllMacDevices().subscribe(mac => {
          this.macList = [];
            mac.forEach(m => {
                this.macList.push(m._id);
            });
            this.deviceMac = this.macList[0]; // FIXME put it to 0

          this.getTimeline();
        });


        this.canvas = document.getElementById("timelineChart");
        this.ctx = this.canvas.getContext("2d");
        this.timelineChart = new Chart(this.ctx, {
            type: this.scatterChartType,
            data: {
                datasets: this.scatterChartDataSet
            },
            options: this.scatterChartOptions
        });


    }
    doSomething(event) {
      this.scatterChartDataSet[0].data = [];
      this.data = moment.unix(+this.positionDevice[event]._id).format('DD/MM/YY HH:mm:ss');

      (this.scatterChartDataSet[0].data as ChartPoint[]).push({x: this.positionDevice[event].x , y: this.positionDevice[event].y, t: this.positionDevice[event]._id});
    }

}
