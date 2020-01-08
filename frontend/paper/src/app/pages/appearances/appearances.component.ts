import {ChangeDetectorRef, Component, OnInit} from '@angular/core';
import {ApiService, DeviceHistorical} from '../../service/api/api.service';

declare interface TableData {
    headerRow: string[];
    dataRows: string[][];
}

@Component({
    selector: 'table-cmp',
    moduleId: module.id,
    templateUrl: 'appearances.component.html'
})

export class AppearancesComponent implements OnInit {

    public macList: string[];
    public timeValue: number = 1;
    timeWindowValues = ['minutes', 'hours', 'days', 'months']
    public timeWindow: string = "hours";
    public devicesHistorical: DeviceHistorical[];

    constructor(private apiService: ApiService, private changeDetectorRefs: ChangeDetectorRef) {
    }

    getAppearances(): void {
        console.log(`Retrieving appearances over the last ${this.timeValue} ${this.timeWindow}`);
        if (this.timeWindow === 'hours') {
            this.apiService.getDevicesHistorical(String((this.timeValue * 60))).subscribe(devices => {
                this.devicesHistorical = devices;
                console.log(this.devicesHistorical);
                this.changeDetectorRefs.detectChanges();
            });
        }
        if (this.timeWindow === 'days') {
            this.apiService.getDevicesHistorical(String((this.timeValue * 60 * 24))).subscribe(devices => {
                this.devicesHistorical = devices;
                console.log(this.devicesHistorical);
                this.changeDetectorRefs.detectChanges();
            });
        }
        if (this.timeWindow === 'minutes') {
            this.apiService.getDevicesHistorical(String(this.timeValue)).subscribe(devices => {
                this.devicesHistorical = devices;
                console.log(this.devicesHistorical);
                this.changeDetectorRefs.detectChanges();
            });
        }
    }

    ngOnInit(): void {
        this.apiService.getAllMacDevices().subscribe( mac => {
            this.macList = [];
            mac.forEach( m => { console.log(m._id); this.macList.push(m._id); });
        });
    }
}
