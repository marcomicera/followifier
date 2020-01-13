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
    timeWindowValues = ['minutes', 'hours', 'days']
    public timeWindow: string = "hours";
    public devicesHistorical: DeviceHistorical[];

    constructor(private apiService: ApiService, private changeDetectorRefs: ChangeDetectorRef) {
    }

    getAppearances(): void {
        let minutesInWindow = new Map([
            ['minutes', 1],
            ['hours', 60],
            ['days', 60 * 24]
        ]);
        this.apiService.getDevicesHistorical(String(this.timeValue * minutesInWindow.get(this.timeWindow)))
            .subscribe(devices => {
                this.devicesHistorical = devices.sort((e1, e2) => {
                    if (e1.n > e2.n) {
                        return -1;
                    }
                    if (e1.n < e2.n) {
                        return 1;
                    }
                    return 0;
                });
                this.changeDetectorRefs.detectChanges();
            });
    }

    ngOnInit(): void {
        this.getAppearances();
    }
}
