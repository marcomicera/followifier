import {ChangeDetectorRef, Component} from '@angular/core';
import {
    Chart,
    ChartDataSets,
    ChartOptions,
    ChartPoint,
    ChartTooltipCallback,
    ChartTooltipOptions,
    ChartType
} from "chart.js";
import {ApiService} from '../../service/api/api.service';

@Component({
    selector: 'timeline-cmp',
    moduleId: module.id,
    templateUrl: 'timeline.component.html'
})

export class TimelineComponent {

    public canvas: any;
    public ctx;

    public macList: string[];
    private deviceMac: string;

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
            return ((data.datasets[item[0].datasetIndex].data[item[0].index] as ChartPoint).t as string);
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
        tooltips: this.scatterToolTipOptions,
    };

    constructor(private apiService: ApiService, private changeDetectorRefs: ChangeDetectorRef) {
    }

    getTimeline(): void {
        this.apiService.getAllMacPosition(this.deviceMac).subscribe(positions => {
            console.log(`Device ${this.deviceMac} positions:`);
            console.dir(positions);
            this.scatterChartDataSet[0].data = [];
            positions.forEach(p => {
                (this.scatterChartDataSet[0].data as ChartPoint[]).push({x: p.x ,y: p.y, t: p._id});
            });
        });
    }

    ngOnInit() {

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
}
