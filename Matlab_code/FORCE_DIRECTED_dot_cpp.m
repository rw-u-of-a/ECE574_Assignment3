function OUTPUT_SCHEDULE = FORCE_DIRECTED_dot_cpp(operations, Latency)

in = 0;        % input nop
out = inf;     % output nop

if nargin == 0
    % Test Case and input discription
    % Values got from the FDS example in the HLSM_Scheduling slides
    
    % number each operation node
    % order doesn't matter as long as it's consistant
    operations.numbers = 1:11;
    
    % a record of all of the 'children' of each operation
    % a 'child' is a an operation that will depend on the current operation
    % operations.children{5} will be an array of all of the 'children' for
    % the operation that was given the node number '5'
    operations.children = {[3] ...
        [3] ...
        [4] ...
        [5] ...
        [out] ...
        [7] ...
        [5] ...
        [9] ...
        [out] ...
        [11] ...
        [out] ...
        };
    
    % a record of all of the 'parents' of each operation
    % a 'parent' is a an operation that the current operation depends on
    % operations.parents{5} will be an array of all of the 'parents' for
    % the operation that was given the node number '5'
    operations.parents = {[in] ...
        [in] ...
        [1, 2] ...
        [3] ...
        [4, 7] ...
        [in] ...
        [6] ...
        [in] ...
        [8] ...
        [in] ...
        [10] ...
        };
    
    % a record of which resource each node is using
    % Let's say the resource type is an enumuration with ALU = 1 and
    % MULT = 2, for this test case
    operations.resource = [ 2 ...
        2 ...
        2 ...
        1 ...
        1 ...
        2 ...
        2 ...
        2 ...
        1 ...
        1 ...
        1 ...
        ];
    
    % a record of which operations have been scheduled already
    % should always start out with everything as false
    operations.scheduled = [ false ...
        false ...
        false ...
        false ...
        false ...
        false ...
        false ...
        false ...
        false ...
        false ...
        false ...
        ];
    
    % Latency
    Latency = 6;
    % Latency was 4 in the in-class example. but that doesn't work when
    % MULT's have a latency of 2
    
end

all_operations_scheduled = false;

%init output
% the output of this function is a cell array where each cell represents
% a time cycle, and the contents of the cell is an array of all the
% nodes that should be scheduled in that cycle
% OUTPUT_SCHEDULE = cell(Latency,1);
OUTPUT_SCHEDULE = zeros(numel(operations.numbers));


% ALU is resource 1 with latency of 1 cycle
% MULT is resource 2 with latency of 2 cycles
% DIV/MOD is resource 3 with latency of 3 cycles
Resource_Latency = [ 1 ...
    2 ...
    3 ...
    ];


%% Compute the time frames
Time_Frames = zeros(numel(operations.numbers), 2);

%ASAP Schedule

% ASAP = cell(Latency,1);
ASAP = -1*ones(Latency,numel(operations.numbers));
parent_count = zeros(numel(operations.numbers),1);
op_num = 1;

% loop through each node
for v = 1:numel(operations.numbers)
    % find how many generations of parents this node has
    parent_count(operations.numbers(v)) = count_parents(operations.numbers(v), parent_count(operations.numbers(v)), operations, Resource_Latency);

    if parent_count(operations.numbers(v)) < Latency
        % the number of generations of parents is the location in the ASAP Schedule
        op_num = numel(operations.numbers) - sum(ASAP(parent_count(operations.numbers(v))+1,:) > -1) + 1;
        ASAP(parent_count(operations.numbers(v))+1,op_num) = operations.numbers(v);
        Time_Frames(operations.numbers(v),1) = parent_count(operations.numbers(v))+1; % First part of Time Frame is Soonest possible time
    else
        %disp(['Cant Schedule Node #' num2str(v) ' in ASAP'])
        disp(['Cant Schedule This Node in ASAP'])
    end
end



%ALAP Schedule

% ALAP = cell(Latency,1);
ALAP = -1*ones(Latency,numel(operations.numbers))
child_count = zeros(numel(operations.numbers),1);
op_num = 1;

% Look for operations that have no children
for v = 1:numel(operations.numbers)
    % find how many generations of children this node has
    child_count(operations.numbers(v)) = count_children(operations.numbers(v), child_count(operations.numbers(v)), operations, Resource_Latency);
    
    if child_count(operations.numbers(v)) < Latency
        % the number of generations of children is the reverse location in the ALAP Schedule
        op_num = numel(operations.numbers) - sum(ALAP(Latency - child_count(operations.numbers(v)),:) > -1) + 1;
        ALAP(Latency - child_count(operations.numbers(v)),op_num) = operations.numbers(v);
        Time_Frames(operations.numbers(v),2) = Latency - child_count(operations.numbers(v)); % Second part of Time Frame is Latest possible time
    else
        %disp(['Cant Schedule Node #' num2str(v) ' in ALAP'])
        disp(['Cant Schedule This Node in ASAP'])
    end
end


while ~all_operations_scheduled
    
    %% Compute the operations and type probabilities
    Operation_Probability = zeros(numel(operations.numbers), Latency);
    Type_Probability = zeros(numel(unique(operations.resource)), Latency);
    
    for t = 1:Latency      %Loop through each cycle
        for v = 1:numel(operations.numbers)  %Loop through each operations
            % if the current cycle is within the time frame for this operation
            if t >= Time_Frames(v,1) && t <= Time_Frames(v,2)
                % then the probability is the recipricol of the range of the Time Frame
                Operation_Probability(v,t) = 1/(Time_Frames(v,2) - Time_Frames(v,1) + 1);
            else
                % else the probablility is 0
                Operation_Probability(v,t) = 0;
            end
            Type_Probability(operations.resource(v),t) = Operation_Probability(v,t) + Type_Probability(operations.resource(v),t); % +=
        end
    end
    
    
    %% Compute the self-forces, predecessor/succesor forces and total forces
    % Calculate All Self Forces
    Self_Force = zeros(numel(operations.numbers), Latency);
    
    for t = 1:Latency      %Loop through each cycle
        for v = 1:numel(operations.numbers)  %Loop through each operations
            % if the current cycle is within the time frame for this operation
            if t >= Time_Frames(v,1) && t <= Time_Frames(v,2)
                for c = 1:Latency %Loop through each cycle again
                    Self_Force(v, t) = Type_Probability(operations.resource(v),c)*((c == t) - Operation_Probability(v,c)) + Self_Force(v, t); % +=
                end
            else
                % set self force to Inf becuase operation can't be scheduled this cycle
                Self_Force(v, t) = inf;
            end
        end
    end
    
    % Calculate All Predecessor/Successor Forces
    Predecessor_Force = zeros(numel(operations.numbers), Latency);
    Successor_Force = zeros(numel(operations.numbers), Latency);
    
    for t = 1:Latency           %Loop through each cycle
        for v = 1:numel(operations.numbers)     %Loop through each operations
            % if the current cycle is within the time frame for this operation
            if t >= Time_Frames(v,1) && t <= Time_Frames(v,2)
                for p = 1:numel(operations.parents(v,:))  %Loop through all of this operation's parents
                    if p>0
                        % will scheduling v at time t effect the schedule for p?
                        if t >= Time_Frames(p,1) && t <= Time_Frames(p,2)
                            
                            % Find all the options for scheduling the parent
                            time_options = Time_Frames(p,1):Time_Frames(p,2);   % all times when p can be scheduled
                            time_options = time_options(time_options ~= t);     % except the current time t
                            
                            %Loop through all these options to add up the force
                            for o = 1:numel(time_options)
                                % add the self force of this parent at this t
                                Predecessor_Force(v, t) = Self_Force(p, time_options(o)) +  Predecessor_Force(v, t); % +=
                            end
                            
                        else
                            % Parent uneffected
                            Predecessor_Force(v, t) = 0 + Predecessor_Force(v, t);  % temporary for bookkeeping
                        end
                    else
                        Predecessor_Force(v, t) = 0 + Predecessor_Force(v, t);  % temporary for bookkeeping
                    end
                end
                
                for c = 1:numel(operations.children(v,:))  %Loop through all of this operation's children
                    if c<inf
                        % will scheduling v at time t effect the schedule for p?
                        if t >= Time_Frames(c,1) && t <= Time_Frames(c,2)
                            
                            % Find all the options for scheduling the child
                            time_options = Time_Frames(c,1):Time_Frames(c,2);   % all times when p can be scheduled
                            time_options = time_options(time_options ~= t);     % except the current time t
                            
                            %Loop through all these options to add up the force
                            for o = 1:numel(time_options)
                                % add the self force of this child at this t
                                Successor_Force(v, t) = Self_Force(c, time_options(o)) +  Successor_Force(v, t); % +=
                            end
                            
                        else
                            % Parent uneffected
                            Successor_Force(v, t) = 0 + Successor_Force(v, t);  % temporary for bookkeeping
                        end
                    else
                        Successor_Force(v, t) = 0 + Successor_Force(v, t);  % temporary for bookkeeping
                    end
                end
                
            else
                Predecessor_Force(v, t) = 0;
                Successor_Force(v, t) = 0;
            end
        end
    end
    
    Total_Force = Self_Force + Predecessor_Force + Successor_Force;
    
    %I would sometimes get really small floating point umbers for Total
    %Force, so that would screw up the min force calculation
    %Total_Force = round(Total_Force, 5); % this rounds Total_Force to 5 decimal places, may not be nessesary
    
    %% Schedule the operations with the least force and update its time-frame
    
    [min_force_operation, min_force_cycle] = find(Total_Force == min(min(Total_Force)));    % need to find alternative to the 'find' function that will work with c++
    
    % if there are multple min Total Force, just pick the first one found that wasn't already scheduled
    for o = 1:numel(min_force_operation)
        if ~operations.scheduled(min_force_operation(o))
            min_force_operation =  min_force_operation(o);
            min_force_cycle = min_force_cycle(o);
            break;
        end
    end
    
    % add the minimum Force node to the Output Schedule
%     OUTPUT_SCHEDULE{min_force_cycle} = [min_force_operation, OUTPUT_SCHEDULE{min_force_cycle}];
    out_index = 1;
    out_index = find(operations.numbers == min_force_operation);
    OUTPUT_SCHEDULE(out_index) = min_force_cycle;
    
    min_force_operation_int = 0;
    min_force_operation_int = min_force_operation(1);
    
    min_force_cycle_int = 0;
    min_force_cycle_int = min_force_cycle(1);
    
    % mark it as scheduled
    operations.scheduled(min_force_operation_int) = true;
    
    % adjust the Time_Frames to lock that node in place
    Time_Frames(min_force_operation_int,1) = min_force_cycle_int;
    Time_Frames(min_force_operation_int,2) = min_force_cycle_int;
    
    % update the depedant Time_Frames (parents/children)
    update_parent_Time(min_force_operation_int, min_force_cycle_int, operations, Time_Frames);
    update_child_Time(min_force_operation_int, min_force_cycle_int, operations, Time_Frames);
    
    
    all_operations_scheduled = all(operations.scheduled);
    
end

end

%% Other Functions
function parent_count = count_parents(v, parent_count, operations, Resource_Latency)
local_family = operations.parents(v,:);
if sum(local_family) == 0   %only has "in" as parent
    return;
else
    local_parent_count = zeros(numel(local_family),1) + parent_count;
    for f = 1:numel(local_family)
        local_parent_count(local_family(f)) = count_parents(local_family(f), local_parent_count(local_family(f)) + ...
            1*Resource_Latency(operations.resource(v)),  operations, Resource_Latency );
    end
end
parent_count = max(local_parent_count);
end

function child_count = count_children(v, child_count, operations, Resource_Latency)
local_family = operations.children(v,:);
if all(local_family == inf)   %only has "out" as child
    return;
else
    local_child_count = zeros(numel(local_family),1) + child_count;
    for f = 1:numel(local_family)
        local_child_count(local_family(f)) = count_children(local_family(f), local_child_count(local_family(f)) + ...
            1*Resource_Latency(operations.resource(v)),  operations, Resource_Latency);
    end
end
child_count = max(local_child_count);
end

function update_parent_Time(v, t, operations, Time_Frames)
local_family = zeros(numel(operations.parents(v,:)),1);
local_family = operations.parents(v,:);
for f = 1:numel(local_family)
    if local_family(f)>0
        % if this parent was effected by the sceduled operations
        if t >= Time_Frames(local_family(f),1) && t <= Time_Frames(local_family(f),2)
            % then the latest possible time has changed for the parent
            Time_Frames(local_family(f),2) = Time_Frames(v,1)-1;
            update_parent_Time(local_family(f), Time_Frames(local_family(f),2), operations, Time_Frames);
        end
    end
end
end

function update_child_Time(v, t, operations, Time_Frames)
local_family = operations.children(v,:);
for f = 1:numel(local_family)
    if local_family(f)<inf
        % if this child was effected by the sceduled operations
        if t >= Time_Frames(local_family(f),1) && t <= Time_Frames(local_family(f),2)
            % then the latest possible time has changed for the child
            Time_Frames(local_family(f),1) = Time_Frames(v,2)+1;
            update_child_Time(local_family(f), Time_Frames(local_family(f),1), operations, Time_Frames);
        end
    end
end
end
