import { Routes } from '@angular/router';

import { DashboardComponent } from '../../pages/dashboard/dashboard.component';
import { AppearancesComponent } from '../../pages/appearances/appearances.component';
import { TimelineComponent } from '../../pages/timeline/timeline.component';

export const AdminLayoutRoutes: Routes = [
    { path: 'dashboard',      component: DashboardComponent },
    { path: 'appearances',    component: AppearancesComponent },
    { path: 'timeline',     component: TimelineComponent }
];
