#pragma once

#include "TeCorePrerequisites.h"
#include "Scene/TeComponent.h"
#include "Input/TeVirtualInput.h"
#include "Math/TeVector3.h"

namespace te
{
    /** Component that controls movement and rotation of the scene object it's attached to. */
    class TE_CORE_EXPORT CCameraUI : public Component
    {
    public:
        CCameraUI(const HSceneObject& parent);

        /** Triggered once per frame. Allows the component to handle input and move. */
        void Update() override;

        /** Enables or disables camera controls. */
        void EnableInput(bool enable);

        /** For rotation, we want to know the current reference position. */
        void SetTarget(const Vector3& target);

    public:
        static const String ROTATE_BINDING;
        static const String MOVE_BINDING;
        static const String ZOOM_BINDING;

        static const String HORIZONTAL_AXIS_BINDING;
        static const String VERTICAL_AXIS_BINDING;
        static const String SCROLL_AXIS_BINDING;

        static const float ZOOM_SPEED;
        static const float MOVE_SPEED;
        static const float SCROLL_SPEED;
        static const float TOP_SCROLL_SPEED;
        static const float ROTATIONAL_SPEED;
        static const float MAX_RIGHT_ANGLE;
        static const float MAX_ZOOM_SPEED;
        static const float MAX_ZOOM;
        static const float MIN_ZOOM;

    protected:
        void InitDistanceToTarget();
        void InitLocalRotation();

    protected:
        HCamera _camera;
        Vector3 _target;
        Vector3 _localRotation;
        float _distanceToTarget;

        bool _inputEnabled;
        bool _lastHideCursorState;

        VirtualButton _rotateBtn;
        VirtualButton _moveBtn;
        VirtualButton _zoomBtn;

        VirtualAxis _horizontalAxis;
        VirtualAxis _verticalAxis;
        VirtualAxis _scrollAxis;

        
    };
}
