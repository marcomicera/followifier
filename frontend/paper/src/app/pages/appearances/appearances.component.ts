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
    public value: string;
    public devicesHistorical: DeviceHistorical[];
    public selected;

    constructor(private apiService: ApiService, private changeDetectorRefs: ChangeDetectorRef) {
    }

    getAppearances(): void {
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
    }

    ngOnInit(): void {
        this.apiService.getAllMacDevices().subscribe( mac => {
            this.macList = [];
            mac.forEach( m => { console.log(m._id); this.macList.push(m._id); });
        });
    }
}
