import {NgModule} from '@angular/core';
import {RouterModule} from '@angular/router';
import {CommonModule} from '@angular/common';
import {FormsModule} from '@angular/forms';

import {AdminLayoutRoutes} from './admin-layout.routing';

import {DashboardComponent} from '../../pages/dashboard/dashboard.component';
import {AppearancesComponent} from '../../pages/appearances/appearances.component';
import {TimelineComponent} from '../../pages/timeline/timeline.component';
import {ChartsModule} from 'ng2-charts';

import {NgbModule} from '@ng-bootstrap/ng-bootstrap';
import {MatSliderModule} from '@angular/material/slider';
import {MatFormFieldModule} from '@angular/material/form-field';
import {DateTimePickerModule} from '@syncfusion/ej2-angular-calendars';
import { OwlDateTimeModule, OwlNativeDateTimeModule } from 'ng-pick-datetime';
import {MatButtonModule} from '@angular/material/button';

@NgModule({
  imports: [
    ChartsModule,
    CommonModule,
    RouterModule.forChild(AdminLayoutRoutes),
    FormsModule,
    NgbModule,
    MatSliderModule,
    MatFormFieldModule,
    DateTimePickerModule,
    OwlDateTimeModule,
    OwlNativeDateTimeModule,
    MatButtonModule
  ],
    declarations: [
        DashboardComponent,
        AppearancesComponent,
        TimelineComponent,
    ]
})

export class AdminLayoutModule {
}
